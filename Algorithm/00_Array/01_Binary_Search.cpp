/*
二分查找区间有两种形式
一种是
一种是左闭右开
*/
#include <vector>
// 左闭右闭
int binarySearch(vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            return mid;
        }
        else if (nums[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1;
}

// 左闭右开
int binarySearch1(vector<int>& nums, int target) {
    int left1 = 0;
    int right1 = nums.size() - 1;

    while(left1 < right1) {
        int mid1 = left1 + (right1 - left1);
        if (nums[mid1] == target) {
            return target;
        }
        else if (nums[mid1] < target) {
            left1 = mid1 + 1;
        }
        else {
            right1 = mid1;
        }
    }
    return -1;
}