#ifndef _stuff_h_
#define _stuff_h_

#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <array>
#include <tuple>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iterator>
#include <cassert>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
	out << '{' << v.size() << '}' << '[';
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ","));
	if (!v.empty()) out << "\b";
	out << "]";
	return out;
}

#endif // _stuff_h_
