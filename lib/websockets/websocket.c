
#include <nopoll/nopoll.h>
#include "ssl_compat-1.0.h"
#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

nopoll_bool debug = nopoll_false;
nopoll_bool show_critical_only = nopoll_false;
LOCAL xQueueHandle	Web_QueueStop = NULL;
const char *ping_msg =  "{\"path\": \"/v1/ping/\", \"method\": \"GET\"}";

#define local_host_name		"iot.espressif.cn"
#define local_host_url		"v1/datastreams/tem_hum/datapoint"
#define local_host_port		"9000"
#define local_host_ports	"9443"

void __report_critical (noPollCtx * ctx, noPollDebugLevel level, const char * log_msg, noPollPtr user_data)
{
        if (level == NOPOLL_LEVEL_CRITICAL) {
  	        printf ("CRITICAL: %s\n", log_msg);
	}
	return;
}

noPollCtx * create_ctx (void) {
	
	/* create a context */
	noPollCtx * ctx = nopoll_ctx_new ();
	nopoll_log_enable (ctx, debug);
	nopoll_log_color_enable (ctx, debug);

	/* configure handler */
	if (show_critical_only)
	        nopoll_log_set_handler (ctx, __report_critical, NULL);
	return ctx;
}

nopoll_bool test_01 (void) {
	noPollCtx  * ctx;
	noPollConn * conn;
	noPollMsg  * msg;
	int          iter;

	/* create context */
	ctx = create_ctx ();

	/* check connections registered */
	if (nopoll_ctx_conns (ctx) != 0) {
		printf ("ERROR: expected to find 0 registered connections but found: %d\n", nopoll_ctx_conns (ctx));
		return nopoll_false;
	} /* end if */

	nopoll_ctx_unref (ctx);

	/* reinit again */
	ctx = create_ctx ();

	/* call to create a connection */
	conn = nopoll_conn_new (ctx, local_host_name, local_host_port, NULL, "/ws", NULL, NULL);
	if (! nopoll_conn_is_ok (conn)) {
		printf ("ERROR: Expected to find proper client connection status, but found error.. (conn=%p, conn->session=%d, NOPOLL_INVALID_SOCKET=%d, errno=%d, strerr=%s)..\n",
			conn, (int) nopoll_conn_socket (conn), (int) NOPOLL_INVALID_SOCKET, errno, strerror (errno));
		return nopoll_false;
	}

	printf ("Test 01: sending basic content..\n");

	/* send content text(utf-8) */
	if (nopoll_conn_send_text (conn, ping_msg,strlen(ping_msg)) != strlen(ping_msg)) {
		printf ("ERROR: Expected to find proper send operation..\n");
		return nopoll_false;
	}

	/* wait for the reply */
	iter = 0;
	while ((msg = nopoll_conn_get_msg (conn)) == NULL) {

		if (! nopoll_conn_is_ok (conn)) {
			printf ("ERROR: received websocket connection close during wait reply..\n");
			return nopoll_false;
		}

		nopoll_sleep (10000);

		if (iter > 10)
			break;
	} /* end if */

	printf("Recieve data:%s\n",(const char *) nopoll_msg_get_payload (msg));

	/* unref message */
	nopoll_msg_unref (msg);

	/* finish connection */
	nopoll_conn_close (conn);
	
	/* finish */
	nopoll_ctx_unref (ctx);

	return nopoll_true;
}

nopoll_bool test_02 (void) {

	noPollCtx      * ctx;
	noPollConn     * conn;
	noPollConnOpts * opts;
	noPollMsg  * msg;
	int iter;
	/* reinit again */
	ctx = create_ctx ();
	/* disable verification */
	opts = nopoll_conn_opts_new ();
	nopoll_conn_opts_ssl_peer_verify (opts, nopoll_false);
	/* call to create a connection */
	conn = nopoll_conn_tls_new (ctx, opts, local_host_name, local_host_ports, NULL, "/ws", NULL, NULL);
	if (! nopoll_conn_is_ok (conn)) {
		printf ("ERROR: Expected to find proper client connection status, but found error..\n");
		return nopoll_false;
	} /* end if */
	/* check if the connection already finished its connection
	   handshake */
	while (! nopoll_conn_is_ready (conn)) {

		if (! nopoll_conn_is_ok (conn)) {
			printf ("ERROR (4.1 dk45): expected to find proper connection handshake finished, but found connection is broken: session=%d, errno=%d : %s..\n",
				(int) nopoll_conn_socket (conn), errno, strerror (errno));
			return nopoll_false;
		} /* end if */

		/* wait a bit 10ms */
		nopoll_sleep (10000);
	} /* end if */

	printf ("Test 02: testing sending TLS content over the wire..\n");

	if (nopoll_conn_send_text (conn, ping_msg ,strlen(ping_msg)) != strlen(ping_msg)) {
		printf ("ERROR: Expected to find proper send operation..\n");
		return nopoll_false;
	}

	/* wait for the reply */
	iter = 0;
	while ((msg = nopoll_conn_get_msg (conn)) == NULL) {

		if (! nopoll_conn_is_ok (conn)) {
			printf ("ERROR: received websocket connection close during wait reply..\n");
			return nopoll_false;
		}

		nopoll_sleep (10000);

		if (iter > 10)
			break;
	} /* end if */
	printf("Recieve data:%s\n",(const char *) nopoll_msg_get_payload (msg));
	/* unref message */
	nopoll_msg_unref (msg);

	/* finish connection */
	nopoll_conn_close (conn);
	
	/* finish */
	nopoll_ctx_unref (ctx);

	return nopoll_true;
}

LOCAL int websocket_main (char *argv)
{

	int iterator = 2;
	printf("interator:%d\n",iterator);
	switch (iterator) {

		case 1:
			if (test_01()) {
				printf("Test 01: Simple request/reply [   OK   ]\n");
			} else {
				printf("Test 01: Simple request/reply [ FAILED ]\n");
			}

			break;

		case 2:
			if (test_02()) {
				printf("Test 02: testing TLS request/reply [   OK   ]\n");
			} else {
				printf("Test 02: testing TLS request/reply [ FAILED ]\n");
			}

			break;

		default:
			break;
	}

	/* call to cleanup */
	nopoll_cleanup_library ();
	printf ("All tests ok!!\n");

	return 0;
}

LOCAL void websocket_task(void *pvParameters)
{
	bool ValueFromReceive = false;
	portBASE_TYPE xStatus;
	struct ip_info ip_config;
	struct station_config sta_config;
	bzero(&sta_config, sizeof(struct station_config));

    printf("pvParameters = %d\n", *(int*)pvParameters);

    // Configurar WiFi
	sprintf(sta_config.ssid, "AAA");
	sprintf(sta_config.password, "aaa");
	wifi_station_set_config(&sta_config);
	os_printf("%s\n", __func__);
	wifi_get_ip_info(STATION_IF, &ip_config);
	printf("Delaying...\n");
	vTaskDelay(2000 / portTICK_RATE_MS);
	
    // Iniciar socket
    websocket_main((char*)pvParameters);
	while (1) {
		xStatus = xQueueReceive(Web_QueueStop,&ValueFromReceive,0);
		if (xStatus == pdPASS && ValueFromReceive == true){
			printf("websocket_task exit signal\n");
			break;
		}
		vTaskDelay(3000 / portTICK_RATE_MS);
		printf("websocket_task\n");
	}
	vQueueDelete(Web_QueueStop);
	Web_QueueStop = NULL;
	vTaskDelete(NULL);
	printf("delete the websocket_task\n");
}

/*start the websocket task*/
void websocket_start(void *optarg)
{
	if (Web_QueueStop == NULL)
		Web_QueueStop = xQueueCreate(1,1);

	if (Web_QueueStop != NULL)
        printf("optarg = %d\n", *(int*)optarg);
		xTaskCreate(websocket_task, "websocket_task", 2048, optarg, 4, NULL);
}

/*stop the websocket task*/
int8_t websocket_stop(void)
{
	bool ValueToSend = true;
	portBASE_TYPE xStatus;

	if (Web_QueueStop == NULL)
		return -1;

	xStatus = xQueueSend(Web_QueueStop,&ValueToSend,0);
	if (xStatus != pdPASS)
		return -1;
	else
		return pdPASS;
}
/* end-of-file-found */
