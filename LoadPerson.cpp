struct PersonBuilder {
    // Person connection info
    string_view name;
    int connection_timeout;
    bool allow_exceptions;
    DBLogLevel log_level;

    // Person filter
    int min_age;
    int max_age;
    string_view name_filter;
};

vector<Person> LoadPersons(const PersonBuilder& builder) {
    DBConnector connector(builder.allow_exceptions, builder.og_level);
    DBHandler db;
    if (builder.name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(builder.name, builder.connection_timeout);
    } else {
        db = connector.Connect(builder.name, builder.connection_timeout);
    }
    if (!builder.allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << builder.min_age << " and "s << builder.max_age << " "s
              << "and Name like '%"s << db.Quote(builder.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}