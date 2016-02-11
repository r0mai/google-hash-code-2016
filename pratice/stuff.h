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
std::vector<std::vector<T>> readMatrix(int height, int width) {
	std::vector<std::vector<T>> matrix(height, std::vector<T>(width));
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			std::cin >> matrix[y][x];
		}
	}
	return matrix;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
	out << '{' << v.size() << '}' << '[';
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ","));
	if (!v.empty()) out << "\b";
	out << "]";
	return out;
}

#endif // _stuff_h_
