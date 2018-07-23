void setAlarmH(int* alarm_h, int inc) {
  tm1637.point(POINT_OFF);
  *alarm_h+=inc;
  if (*alarm_h<0)
    *alarm_h=23;
  else if (*alarm_h>=24)
    *alarm_h = 0;
  
  TimeDisp[0] = *alarm_h/10;
  TimeDisp[1] = *alarm_h % 10;
  TimeDisp[2] = 0;
  TimeDisp[3] = 0;
  tm1637.display(TimeDisp);
}

void setAlarmM(int *alarm_m, int inc) {
  tm1637.point(POINT_ON);
  
  *alarm_m+=inc;
  if (*alarm_m<0)
    *alarm_m=59;
  else if (*alarm_m>=60)
    *alarm_m = 0;

  //TimeDisp[0] = 16;
  //TimeDisp[1] = 16;
  TimeDisp[2] = *alarm_m / 10;
  TimeDisp[3] = *alarm_m % 10;
  tm1637.display(TimeDisp);
}

