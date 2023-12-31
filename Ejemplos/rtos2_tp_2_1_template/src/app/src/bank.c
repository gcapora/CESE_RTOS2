/*
 * Copyright (c) YEAR NOMBRE <MAIL>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : task_bank.c
 * @date   : Mar 21, 2023
 * @author : NOMBRE <MAIL>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "driver.h"
#include "client.h"

/********************** macros and definitions *******************************/

#define QUEUE_LENGTH_           (10)
#define QUEUE_ITEM_SIZE_        (sizeof(client_t))

/********************** internal data declaration ****************************/

static struct
{
  QueueHandle_t hclient_queue;
} bank_;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

static void client_callback_(client_t* pnew_client)
{
  if (pdPASS == xQueueSend(bank_.hclient_queue, (void*)pnew_client, 0))
  {
    ELOG("Ingresa el cliente a la fila");
  }
  else
  {
    ELOG("Error, el cliente no tiene lugar en la fila");
  }
}

static void task_(void *args)
{
  client_t client;

  client_set_callback(client_callback_);

  while (true)
  {
    if (pdPASS == xQueueReceive(bank_.hclient_queue, &client, portMAX_DELAY))
    {
      ELOG("Llamo al cliente:");
      client_process(&client);
      ELOG("Fin de la atención del cliente");
    }
  }
}

/********************** external functions definition ************************/

void bank_init(void)
{
  bank_.hclient_queue = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
  while (NULL == bank_.hclient_queue)
  {
    // error
  }

  BaseType_t status;
  status = xTaskCreate(task_, "task_bank", 128, NULL, tskIDLE_PRIORITY, NULL);
  while (pdPASS != status)
  {
    // error
  }
}

/********************** end of file ******************************************/
