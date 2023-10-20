#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Ebook {
private:
    const int MAX_USERS_NUMBER = 100'000 + 1;
    const int MAX_PAGES_NUMBER = 1'000 + 1;

    const int NO_PAGES_FLAG = -1;
    const int NO_USERS_FLAG = 0;

public:
    Ebook()
        : pages_per_user(MAX_USERS_NUMBER, NO_PAGES_FLAG)
        , users_per_page(MAX_PAGES_NUMBER, NO_USERS_FLAG) {
    }

    void Read(int user, int page) {
        for (int i = pages_per_user[user] + 1; i < page + 1; ++i) {
            ++users_per_page[i];
        }
        pages_per_user[user] = page;
    }

    double Cheer(int user) const {
        int page = pages_per_user[user];

        if (page == -1) {
            return 0;
        }

        if (users_per_page[0] == 1) {
            return 1;
        }

        return 1.0 * (users_per_page[0] - users_per_page[page]) / (users_per_page[0] - 1);
    }

private:
    std::vector<int> pages_per_user;
    std::vector<int> users_per_page;
};

class QueryHandler {
public:
    QueryHandler() = default;

    QueryHandler(std::istream& is, std::ostream& os)
        : is(is)
        , os(os) {
    }

    void GetQuery() const {
        using namespace std::string_literals;

        Ebook ebook;
        
        int Q;
        is >> Q;

        for (size_t i = 0u; i < Q; ++i) {
            std::string query;
            is >> query;

            int user;
            is >> user;

            if (query == "READ"s) {
                int page;
                is >> page;
                ebook.Read(user, page);
            }

            else if (query == "CHEER"s) {
                os << std::setprecision(6) << ebook.Cheer(user) << std::endl;
            }
        }
    }

private:
    std::istream& is = std::cin;
    std::ostream& os = std::cout;
};

int main() {
    QueryHandler query_handler;

    query_handler.GetQuery();

    return 0;
}