/* redsocks - transparent TCP-to-proxy redirector
 * Copyright (C) 2007-2011 Leonid Evdokimov <leon@darkk.net.ru>
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "log.h"
#include "redsocks.h"
#include "utils.h"

typedef enum direct_state_t {
	direct_new,
	direct_request_sent,
	direct_reply_came,
	direct_MAX,
} direct_state;

typedef struct direct_req_t {
	uint8_t ver;
	uint8_t cmd;
	uint16_t port;
	uint32_t addr;
	char login[1]; // we need at least zero-byte
} PACKED direct_req;

const int direct_ver = 4;
const int direct_cmd_connect = 1;
const int direct_cmd_bind = 2;

typedef struct direct_reply_t {
	uint8_t ver;
	uint8_t status;
	uint16_t port;
	uint32_t addr;
} PACKED direct_reply;

const int direct_status_ok = 90;
const int direct_status_fail = 91;
const int direct_status_no_ident = 92;
const int direct_status_fake_ident = 93;


void direct_client_init(redsocks_client *client)
{
  /* STUB */
}


static void direct_read_cb(struct bufferevent *buffev, void *_arg)
{
	redsocks_client *client = _arg;
	redsocks_touch_client(client);
  redsocks_start_relay(client);
}

static void direct_write_cb(struct bufferevent *buffev, void *_arg)
{
	redsocks_client *client = _arg;
	redsocks_touch_client(client);
  redsocks_start_relay(client);
}

void direct_connect_relay(redsocks_client *client)
{
	client->relay = red_connect_relay(&client->destaddr,
			                          redsocks_relay_connected, redsocks_event_error, client);
	if (!client->relay) {
		redsocks_log_errno(client, LOG_ERR, "red_connect_relay");
		redsocks_drop_client(client);
	}
}

relay_subsys direct_subsys =
{
	.name                 = "direct",
	.payload_len          = 0,
	.instance_payload_len = 0,
	.readcb               = direct_read_cb,
	.writecb              = direct_write_cb,
	.init                 = direct_client_init,
  .connect_relay        = direct_connect_relay,
};


/* vim:set tabstop=4 softtabstop=4 shiftwidth=4: */
/* vim:set foldmethod=marker foldlevel=32 foldmarker={,}: */
