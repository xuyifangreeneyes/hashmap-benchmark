#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <sparsehash/dense_hash_map>

constexpr std::int64_t SEED = 1234;
std::mt19937_64 gen(SEED);

constexpr std::size_t QUERIES = 1000;

std::vector<std::int64_t> get_random_full_ints(std::size_t num, std::int64_t min = 0, std::int64_t max = std::numeric_limits<std::int64_t>::max()) {
  std::uniform_int_distribution<std::int64_t> dist(min, max);
  std::vector<std::int64_t> ints;
  ints.reserve(num);
  for (std::size_t i = 0; i < num; ++i) {
    ints.push_back(dist(gen));
  }
  return ints;
}


void test_std_unordered_map(std::size_t num) {
  std::unordered_map<std::int64_t, std::int64_t> map;
  std::vector<std::int64_t> keys = get_random_full_ints(num);
  for (std::int64_t key : keys) {
    map.insert({key, 1});
  }

  std::shuffle(keys.begin(), keys.end(), gen);

  auto start_time = std::chrono::high_resolution_clock::now();
  for (std::size_t i = 0; i < QUERIES; ++i) {
    if (map.find(keys[i]) == map.end()) {
      std::cout << "the key is supposed to exist!" << std::endl;
      std::exit(1);
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  double duration = std::chrono::duration<double>(end_time - start_time).count();
  std::cout << "std::unordered_map: " << duration / QUERIES << "s" << std::endl;

}

void test_google_dense_hash_map(std::size_t num) {
  google::dense_hash_map<std::int64_t, std::int64_t> map;
  map.set_empty_key(-1);
  map.set_deleted_key(-2);

  std::vector<std::int64_t> keys = get_random_full_ints(num);
  for (std::int64_t key : keys) {
    map.insert({key, 1});
  }

  std::shuffle(keys.begin(), keys.end(), gen);

  auto start_time = std::chrono::high_resolution_clock::now();
  for (std::size_t i = 0; i < QUERIES; ++i) {
    if (map.find(keys[i]) == map.end()) {
      std::cout << "the key is supposed to exist!" << std::endl;
      std::exit(1);
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  double duration = std::chrono::duration<double>(end_time - start_time).count();
  std::cout << "google::dense_hash_map: " << duration / QUERIES << "s" << std::endl;

}

int main() {
  std::vector<std::size_t> nums;
  std::size_t num = 1000;
  for (std::size_t i = 0; i < 6; ++i) {
    nums.push_back(num);
    num *= 10;
  }
  for (std::size_t num : nums) {
    std::cout << "num = " << num << std::endl;
    test_std_unordered_map(num);
    test_google_dense_hash_map(num);
  }
  return 0;
}