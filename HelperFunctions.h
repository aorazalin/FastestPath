//
// Created by Asus on 30.05.2021.
//

#ifndef FASTESTPATH_HELPERFUNCTIONS_H
#define FASTESTPATH_HELPERFUNCTIONS_H

#include <vector>
#include <unordered_set>
using namespace std;

template <class Os, class K>
Os& operator<<(Os& os, const std::vector<K>& v) {
    for (const auto& e : v)
        os << e << " ";
    return os;
}

template <class K>
bool operator==(const std::vector<K>& v1, const std::vector<K>& v2) {
    if (v1.size() != v2.size()) return false;
    for (int i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i]) return false;
    }
    return true;
}


class HelperFunctions {
public:
    static vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> us(nums1.begin(), nums1.end());
        vector<int> res;
        for (auto a : nums2) {
            auto it = us.find(a);
            if (it == us.end()) continue;
            res.push_back(a);
            us.erase(it);
        }
        return res;
    }
    static unordered_set<int> set_union(unordered_set<int>& nums1, unordered_set<int> &nums2) {
        unordered_set<int> res;
        for (int el : nums1) res.insert(el);
        for (int el : nums2) res.insert(el);
        return res;
    }
};


#endif //FASTESTPATH_HELPERFUNCTIONS_H
