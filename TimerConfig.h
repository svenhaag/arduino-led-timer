/*
  Define one time range for each timer.
  Only day, hour, minute, second and working day are checked.
  Thus, all other values can be arbitrary.
  RTCTime(day, month, year, hours, minutes, seconds, DayOfWeek, SaveLight)

  I calculated the start times like so:
    - journey from home to destination = 10 min
    - garage = 5 min
    - timer shall start before leaving house = 30 min
    - sum = 45 min, i.e. start timer this amount of minutes before destination must be reached!
*/
TimeRange timeRanges[] = {
  // Morning
  { RTCTime(0, Month::JANUARY, 0, 7, 5, 0, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 7, 30, 0, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_INACTIVE) },
  { RTCTime(0, Month::JANUARY, 0, 7, 5, 0, DayOfWeek::TUESDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 7, 30, 0, DayOfWeek::TUESDAY, SaveLight::SAVING_TIME_INACTIVE) },
  { RTCTime(0, Month::JANUARY, 0, 7, 55, 0, DayOfWeek::WEDNESDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 8, 25, 0, DayOfWeek::WEDNESDAY, SaveLight::SAVING_TIME_INACTIVE) },
  { RTCTime(0, Month::JANUARY, 0, 7, 55, 0, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 8, 25, 0, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_INACTIVE) },
  { RTCTime(0, Month::JANUARY, 0, 7, 5, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 7, 30, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_INACTIVE) },
  
  // Evening
  { RTCTime(0, Month::JANUARY, 0, 19, 0, 0, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 19, 30, 0, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_INACTIVE) },
  { RTCTime(0, Month::JANUARY, 0, 19, 0, 0, DayOfWeek::TUESDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 19, 30, 0, DayOfWeek::TUESDAY, SaveLight::SAVING_TIME_INACTIVE) },
  { RTCTime(0, Month::JANUARY, 0, 19, 0, 0, DayOfWeek::WEDNESDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 19, 30, 0, DayOfWeek::WEDNESDAY, SaveLight::SAVING_TIME_INACTIVE) },
  { RTCTime(0, Month::JANUARY, 0, 19, 0, 0, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 19, 30, 0, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_INACTIVE) },
  { RTCTime(0, Month::JANUARY, 0, 19, 0, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 19, 30, 0, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_INACTIVE) },
  { RTCTime(0, Month::JANUARY, 0, 19, 0, 0, DayOfWeek::SATURDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 19, 30, 0, DayOfWeek::SATURDAY, SaveLight::SAVING_TIME_INACTIVE) },
  { RTCTime(0, Month::JANUARY, 0, 19, 0, 0, DayOfWeek::SUNDAY, SaveLight::SAVING_TIME_INACTIVE),
    RTCTime(0, Month::JANUARY, 0, 19, 30, 0, DayOfWeek::SUNDAY, SaveLight::SAVING_TIME_INACTIVE) },

  // TEST
  // { RTCTime(0, Month::JANUARY, 1, 21, 40, 0, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_INACTIVE),
  //   RTCTime(0, Month::JANUARY, 1, 21, 41, 0, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_INACTIVE) }
};