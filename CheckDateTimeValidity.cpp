void DateAssert(bool expr, const string& message) {
    if (expr) {
        throw domain_error(message);
    }
}

void CheckDateTimeValidity(const DateTime& dt) {
    const auto year = dt.year;
    const auto month = dt.month;
    const auto day = dt.day;
    const auto hour = dt.hour;
    const auto minute = dt.minute;
    const auto second = dt.second;

    DateAssert(year < 1, "year is too small"s);
    DateAssert(year > 9999, "year is too big"s);

    DateAssert(month < 1, "month is too small"s);
    DateAssert(month > 12, "month is too big"s);

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    DateAssert(day < 1, "day is too small"s);
    DateAssert(day > month_lengths[month - 1], "day is too big"s);

    DateAssert(hour < 0, "hour is too small"s);
    DateAssert(hour > 23, "hour is too big"s);

    DateAssert(minute < 0, "minute is too small"s);
    DateAssert(minute > 59, "minute is too big"s);

    DateAssert(second < 0, "second is too small"s);
    DateAssert(second > 59, "second is too big"s);
}