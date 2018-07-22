int curline = 0;

void startLog(void){
    Serial.begin(115200);
}

void debugLog(const String str) {
#ifdef DEBUG
  static String debugMsg;
  curline = 0;
  debugMsg,"DEBUG : "+str;
  Serial.println(debugMsg);
#endif
}

void mainLog(const String str) {
  if (curline==1){
    Serial.println();
    curline = 0;
  }
  static String logMsg;
  logMsg = "Log : "+str;
  Serial.println(logMsg);
}

void mainLog_(const String str) {
  if (!curline) {
    static String logMsg;
    logMsg="Log   : "+str;
    Serial.print(logMsg);
    curline = 1;
  }
  else
    Serial.print(str);
    
}

char * formatMsg(const char *message, ...)
{

    static char result[500]; // must be static otherwise it's freed immediately after returning.
    va_list ap;
    va_start(ap, message);
    vsnprintf(result, sizeof result, message, ap);
    va_end(ap);
    return result;
}

