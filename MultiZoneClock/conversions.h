//Conversions for Day, Date and Time

void getUtcWday(char* utcWday){
  setenv("TZ", "GMT0", 1); tzset(); 
  zones.epoch = time(NULL);
  struct tm *utc_now_tm;
  utc_now_tm = localtime(&zones.epoch);
  char buff[12];
  strftime (buff,12,"[%Z] %a", utc_now_tm);
  strcpy(utcWday, buff);
}

void getUtcDate(char* utcDate){
  setenv("TZ", "GMT0", 1); tzset(); 
  zones.epoch = time(NULL);
  struct tm *utc_now_tm;
  utc_now_tm = localtime(&zones.epoch);
  char buff[10];
  strftime (buff,10,"%d/%m/%y", utc_now_tm);
  strcpy(utcDate, buff);
}


void getUseWday(char* useWday){
  setenv("TZ", "EST5EDT,M3.2.0,M11.1.0", 1); tzset(); 
  zones.epoch = time(NULL);
  struct tm *use_now_tm;
  use_now_tm = localtime(&zones.epoch);
  char buff[12];
  strftime (buff,12,"[%Z] %a", use_now_tm);
  strcpy(useWday, buff);
}

void getAueWday(char* aueWday){
  setenv("TZ", "AEST-10AEDT,M10.1.0,M4.1.0/3", 1); tzset(); 
  zones.epoch = time(NULL);
  struct tm *aue_now_tm;
  aue_now_tm = localtime(&zones.epoch);
  char buff[12];
  strftime (buff,12,"[%Z] %a", aue_now_tm);
  strcpy(aueWday, buff);
}

void getUseDate(char* useDate){
  setenv("TZ", "EST5EDT,M3.2.0,M11.1.0", 1); tzset(); 
  zones.epoch = time(NULL);
  struct tm *use_now_tm;
  use_now_tm = localtime(&zones.epoch);
  char buff[10];
  strftime (buff,10,"%d/%m/%y", use_now_tm);
  strcpy(useDate, buff);
}

void getAueDate(char* aueDate){
  setenv("TZ", "AEST-10AEDT,M10.1.0,M4.1.0/3", 1); tzset(); 
  zones.epoch = time(NULL);
  struct tm *aue_now_tm;
  aue_now_tm = localtime(&zones.epoch);
  char buff[10];
  strftime (buff,10,"%d/%m/%y", aue_now_tm);
  strcpy(aueDate, buff);
}

int getUseHour(){
  int h = 0;
  setenv("TZ", "EST5EDT,M3.2.0,M11.1.0", 1); tzset(); 
  zones.epoch = time(NULL);
  struct tm *use_now_tm;
  use_now_tm = localtime(&zones.epoch);
  char buff[3];
  strftime (buff,3,"%H", use_now_tm);
  sscanf(buff, "%d", &h);
  return h;
}

int getAueHour(){
  int h = 0;
  setenv("TZ", "AEST-10AEDT,M10.1.0,M4.1.0/3", 1); tzset(); 
  zones.epoch = time(NULL);
  struct tm *aue_now_tm;
  aue_now_tm = localtime(&zones.epoch);
  char buff[3];
  strftime (buff,3,"%H", aue_now_tm);
  sscanf(buff, "%d", &h);
  return h;
}