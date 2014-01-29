#ifndef jplay_messaging_h
#define jplay_messaging_h

extern bool js_initialized;

enum AppMessageKey {
  JS_LOADED = 1,
  REQUEST_TRACKS = 2,
  LIVE_ARTIST = 3,
  LIVE_TRACK = 4,
  LOCAL_ARTIST = 5,
  LOCAL_TRACK = 6
};

void app_message_init(void);
void out_sent_handler(DictionaryIterator *sent, void *context);
void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context);
void in_received_handler(DictionaryIterator *received, void *context);
void in_dropped_handler(AppMessageResult reason, void *context);




#endif