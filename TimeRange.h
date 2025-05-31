String dayOfWeek2String(DayOfWeek dow) {
  switch (dow) {
    case DayOfWeek::SUNDAY: return String("SUNDAY");
    case DayOfWeek::MONDAY: return String("MONDAY");
    case DayOfWeek::TUESDAY: return String("TUESDAY");
    case DayOfWeek::WEDNESDAY: return String("WEDNESDAY");
    case DayOfWeek::THURSDAY: return String("THURSDAY");
    case DayOfWeek::FRIDAY: return String("FRIDAY");
    case DayOfWeek::SATURDAY: return String("SATURDAY");
  }
  return String("BEERDAY");
}

struct TimeRange {
  RTCTime start;
  RTCTime end;

  const char* toString() {
    String formattedTime = "[ ";
    formattedTime += dayOfWeek2String(start.getDayOfWeek());
    formattedTime += " ";

    // Start hours
    if (start.getHour() < 10) formattedTime += '0';
    formattedTime += String(start.getHour());
    formattedTime += ":";

    // Start minutes
    if (start.getMinutes() < 10) formattedTime += '0';
    formattedTime += String(start.getMinutes());
    formattedTime += ":";

    // Start seconds
    if (start.getSeconds() < 10) formattedTime += '0';
    formattedTime += String(start.getSeconds());

    formattedTime += " – ";

    // End hours
    if (end.getHour() < 10) formattedTime += '0';
    formattedTime += String(end.getHour());
    formattedTime += ":";

    // End minutes
    if (end.getMinutes() < 10) formattedTime += '0';
    formattedTime += String(end.getMinutes());
    formattedTime += ":";

    // End seconds
    if (end.getSeconds() < 10) formattedTime += '0';
    formattedTime += String(end.getSeconds());

    formattedTime += " ]";
    return formattedTime.c_str();
  }
};