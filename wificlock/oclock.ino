void getTime(char *h){
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  while(!time(nullptr)){
    Serial.print("*");
    delay(1000);
  }
}

void oclock(void) {
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  sprintf(hour,"%02d:%02d:%02d",p_tm->tm_hour,p_tm->tm_min,p_tm->tm_sec);


  debugLog(formatMsg("%s / %s --> %d",hour,prev_hour,strcmp(hour, prev_hour)));

  if (strcmp(hour, prev_hour)==0)
    return;
  strcpy(prev_hour, hour);

  debugLog(hour);

  displayClock(p_tm);
  tm1637.display(TimeDisp);
  ClockPoint = (~ClockPoint) & 0x01;
}

void displayClock(tm* p_tm)
{
  if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF);
  TimeDisp[0] = p_tm->tm_hour / 10;
  if (TimeDisp[0]==0)
    TimeDisp[0] = 16;
  TimeDisp[1] = p_tm->tm_hour % 10;
  TimeDisp[2] = p_tm->tm_min / 10;
  TimeDisp[3] = p_tm->tm_min % 10;
}

