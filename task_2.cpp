#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std::string_view_literals;

class Domain {
public:
    Domain(const std::string& domain) 
        : domain_(domain) {
        domain_.insert(domain_.begin(), '.');
        std::reverse(domain_.begin(), domain_.end());
    }

    bool operator==(const Domain& other) const {
        return std::lexicographical_compare(domain_.begin(), domain_.end(), other.domain_.begin(), other.domain_.end());
    }

    bool IsSubdomain(const Domain& other) const {
        if (domain_.size() < other.domain_.size()) {
            return false;
        }

        if (std::equal(other.domain_.begin(), other.domain_.end(), domain_.begin())) {
            return true;
        }

        return false;
    }
    
    friend class DomainChecker;

private:
    std::string domain_;
};

class DomainChecker {
public:
    template <typename It>
    DomainChecker(It range_begin, It range_end) {
        domains_ = {range_begin, range_end};

        std::sort(domains_.begin(), domains_.end(), [] (const Domain& lhs, const Domain& rhs) { return lhs == rhs; });

        It unique_range_begin = std::unique(domains_.begin(), domains_.end(), [] (const Domain& lhs, const Domain& rhs) { return rhs.IsSubdomain(lhs); });

        domains_.erase(unique_range_begin, domains_.end());
    }

    bool IsForbidden(const Domain& domain) const {
        if (domains_.size() == 0) {
            return false;
        }

        auto it = std::upper_bound(domains_.begin(), domains_.end(), domain, [] (const Domain& lhs, const Domain& rhs) { return lhs.domain_ < rhs.domain_; });

        if (it == domains_.begin()) {
            return domain.IsSubdomain(domains_[it - domains_.begin()]);
        }

        return domain.IsSubdomain(domains_[it - domains_.begin() - 1]);
    }

private:
    std::vector<Domain> domains_;
};

const std::vector<Domain> ReadDomains(std::istream& is, size_t count) {
    std::vector<Domain> domains;
    std::string buff;

    for (size_t i = 0u; i < count; ++i) {
        std::getline(is, buff);
        domains.push_back(buff);
    }

    return domains;
}

template <typename Number>
Number ReadNumberOnLine(std::istream& input) {
    std::string line;
    std::getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    for (const Domain& domain : test_domains) {
        std::cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << std::endl;
    }
}