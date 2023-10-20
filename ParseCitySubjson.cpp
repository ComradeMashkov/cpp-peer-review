struct CountryBuilder {
    CountryBuilder& SetName(const string& name) {
        this->name = name;
        return *this;
    }

    CountryBuilder& SetIsoCode(const string& iso_code) {
        this->iso_code = name;
        return *this;
    }

    CountryBuilder& SetPhoneCode(const string& phone_code) {
        this->phone_code = phone_code;
        return *this;
    }

    CountryBuilder& SetTimeZone(const string& time_zone) {
        this->time_zone = time_zone;
        return *this;
    }

    CountryBuilder& SetLanguages(const vector<Language>& languages) {
        this->languages = languages;
        return *this;
    }

    operator Country() const {
        return { name, iso_code, phone_code, time_zone, languages };
    }

    string name;
    string iso_code;
    string phone_code;
    string time_zone;
    vector<Language>& languages;
};

// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const Json& json, const Country& country) {
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          country.phone_code + city_obj["phone_code"s].AsString(), country.name, country.iso_code,
                          country.time_zone, country.languages});
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();

        CountryBuilder builder = CountryBuilder()
            .SetName(country_obj["name"s].AsString())
            .SetIsoCode(country_obj["iso_code"s].AsString())
            .SetPhoneCode(country_obj["phone_code"s].AsString())
            .SetTimeZone(country_obj["time_zone"s].AsString());

        vector<Language> languages;
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            languages.push_back(FromString<Language>(lang_obj.AsString()));
        }

        builder = builder.SetLanguages(languages);

        countries.push_back(builder);

        ParseCitySubjson(cities, country_obj["cities"s], { country.name, country.iso_code, country.phone_code,
                         country.time_zone, country.languages });
    }
}