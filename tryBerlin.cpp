#include <iostream>
#include <map>
#include <string>
#include <string_view>

using namespace std;

void print_map(string_view comment, const map<string, int>& m)
{
    std::cout << comment;
    // iterate using C++17 facilities
    for (const auto& n : m)
          std::cout << n.first << " = " << n.second << "; ";

    // C++11 alternative:
    //  for (const auto& n : m)
    //      std::cout << n.first << " = " << n.second << "; ";
    //
    // C++98 alternative
    //  for (std::map<std::string, int>::const_iterator it = m.begin(); it != m.end(); it++)
    //      std::cout << it->first << " = " << it->second << "; ";

    std::cout << '\n';
}

int main()
{
    // Create a map of three (string, int) pairs
    map<string, int> m{ {"CPU", 10}, {"GPU", 15}, {"RAM", 20} };

    print_map("1) Initial map: ", m);

    m["CPU"] = 25; // update an existing value
    m["SSD"] = 30; // insert a new value
    print_map("2) Updated map: ", m);

    // using operator[] with non-existent key always performs an insert
    cout << "3) m[UPS] = " << m["UPS"] << '\n';
    print_map("4) Updated map: ", m);

    m.erase("GPU");
    print_map("5) After erase: ", m);

    erase_if(m, [](const auto& pair) { return pair.second > 25; });
    print_map("6) After erase: ", m);
    cout << "7) m.size() = " << m.size() << '\n';

    m.clear();
    cout << boolalpha << "8) Map is empty: " << m.empty() << '\n';
}





#include <map>

template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K, V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
		: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign(K const& keyBegin, K const& keyEnd, V const& val) {
		// If the interval is empty, do nothing.
		if (!(keyBegin < keyEnd)) {
			return;
		}

		// Find the position to insert the new interval or update existing intervals.
		auto itBegin = m_map.lower_bound(keyBegin);
		auto itEnd = m_map.lower_bound(keyEnd);

		// Check if we need to update the value of m_valBegin.
		if (itBegin == m_map.begin() && (itEnd == m_map.end() || itEnd->first != keyEnd)) {
			m_valBegin = val;
		}

		// Erase intervals that are fully contained in the new interval [keyBegin, keyEnd).
		m_map.erase(itBegin, itEnd);

		// Handle the cases where we need to adjust the boundaries of existing intervals.
		if (itBegin != m_map.begin() && (itBegin == m_map.end() || itBegin->first != keyBegin)) {
			itBegin--;
			if (itBegin->second == val) {
				m_map.erase(itBegin);
			}
			else {
				itBegin++;
			}
		}
		if (itEnd != m_map.begin() && itBegin != itEnd && itEnd != m_map.end() && itEnd->first != keyEnd) {
			itEnd--;
			if (itEnd->second == val) {
				m_map.erase(itEnd);
			}
		}

		// Insert the new interval with the given value.
		if (val != m_valBegin) {
			m_map.emplace_hint(itBegin, keyBegin, val);
		}
		if (itEnd != m_map.end() && val != itEnd->second) {
			m_map.emplace_hint(itEnd, keyEnd, itEnd->second);
			itEnd->second = val;
		}
	}

	// look-up of the value associated with key
	V const& operator[](K const& key) const {
		auto it = m_map.upper_bound(key);
		if (it == m_map.begin()) {
			return m_valBegin;
		}
		else {
			return (--it)->second;
		}
	}
};
