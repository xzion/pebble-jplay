#include "pebble.h"
#include "jplay.h"
#include "messaging.h"

static uint8_t nackcount = 0;
bool js_initialized = false;
static char liveText[100];
static char localText[100];


void app_message_init(void) {

	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_sent(out_sent_handler);
	app_message_register_outbox_failed(out_failed_handler);

	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

}

// AppMessage Handlers
void out_sent_handler(DictionaryIterator *sent, void *context) {
   	// Outgoing message was successfully delivered (ACK)
  	//APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Out Sent!");
}


void in_dropped_handler(AppMessageResult reason, void *context) {
	// Incoming message dropped
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message In Dropped! Code: %d", reason);
}


void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	// Outgoing message failed (Received NACK)
	nackcount++;	
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Out Failed! Code: %d, Count: %d", reason, nackcount);
}


void in_received_handler(DictionaryIterator *received, void *context) {
	// Incoming message received
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Got an App Message!!");
	nackcount = 0;

	// Check the dictionary for known keys
	Tuple *load_tuple = dict_find(received, JS_LOADED);
	if (load_tuple) {
		// JS has loaded, call an initial update of stuff!
		APP_LOG(APP_LOG_LEVEL_DEBUG, "JS Loaded Received, calling intial update");

		// Request initial data
		DictionaryIterator *iter;
	    app_message_outbox_begin(&iter);

	    Tuplet value = TupletInteger(REQUEST_TRACKS, 1);
	    dict_write_tuplet(iter, &value);

	    APP_LOG(APP_LOG_LEVEL_DEBUG, "Requesting Tracks!");
	    app_message_outbox_send();

		// Allow other components to send requests
		js_initialized = true;
	}

	Tuple *livetrack_tuple = dict_find(received, LIVE_TRACK);
	Tuple *liveartist_tuple = dict_find(received, LIVE_ARTIST);
	if (livetrack_tuple && liveartist_tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Got new Live Track info");

		snprintf(liveText, 100, "- Live Stream -\n%s\n%s", liveartist_tuple->value->cstring, livetrack_tuple->value->cstring);
		text_layer_set_text(live_layer, liveText);

	}

	Tuple *localtrack_tuple = dict_find(received, LOCAL_TRACK);
	Tuple *localartist_tuple = dict_find(received, LOCAL_ARTIST);
	if (localtrack_tuple && localartist_tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Got new Local Track info");

		snprintf(localText, 100, "- QLD Radio -\n%s\n%s", localartist_tuple->value->cstring, localtrack_tuple->value->cstring);
		text_layer_set_text(local_layer, localText);

	}




}

