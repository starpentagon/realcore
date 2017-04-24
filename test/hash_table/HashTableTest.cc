#include <random>
#include "gtest/gtest.h"

#include "HashTable.h"

static constexpr size_t test_table_space = 1;   // 1MB分確保する

using namespace std;

namespace realcore
{

typedef struct structTestData
{
  structTestData()
  : hash_value(0), value(0), logic_counter(0)
  {}

  HashValue hash_value;
  uint64_t value;
  TableLogicCounter logic_counter;
}TestData;

class HashTableTest
: public ::testing::Test
{
public:
  void LockFreeConstructorTest(){
    HashTable<TestData> hash_table(test_table_space, kLockFree);

    ASSERT_FALSE(hash_table.hash_table_.empty());
    ASSERT_EQ(kLockFree, hash_table.lock_control_);
    ASSERT_FALSE(hash_table.mutex_list_.empty());
    ASSERT_EQ(nullptr, hash_table.mutex_list_[0]);
    ASSERT_EQ(1, hash_table.logic_counter_);
  }

  void LockConstructorTest(){
    HashTable<TestData> hash_table(test_table_space, kLockTable);

    ASSERT_FALSE(hash_table.hash_table_.empty());
    ASSERT_EQ(kLockTable, hash_table.lock_control_);
    ASSERT_FALSE(hash_table.mutex_list_.empty());
    ASSERT_NE(nullptr, hash_table.mutex_list_[0]);
    ASSERT_EQ(1, hash_table.logic_counter_);
  }

  void FindTest(){
    HashTable<TestData> hash_table(test_table_space, kLockTable);

    // find
    constexpr HashValue hash_value = 0;
    TestData data;
    bool is_find = hash_table.find(hash_value, &data);
    ASSERT_FALSE(is_find);
    
    // insert
    data.hash_value = hash_value;
    data.value = 1;
    hash_table.Upsert(hash_value, data);

    // find
    is_find = hash_table.find(hash_value, &data);
    ASSERT_TRUE(is_find);
    ASSERT_EQ(1, data.value);
  }

  void UpsertTest(){
    HashTable<TestData> hash_table(test_table_space, kLockTable);
    constexpr HashValue hash_value = 0;
    
    // insert
    {
      TestData data;
      data.hash_value = hash_value;
      data.value = 1;
      hash_table.Upsert(hash_value, data);
    }

    // find
    {
      TestData data;
      const bool is_find = hash_table.find(hash_value, &data);
      ASSERT_TRUE(is_find);
      ASSERT_EQ(1, data.value);
    }

    // update
    {
      TestData data;
      data.hash_value = hash_value;
      data.value = 2;
      hash_table.Upsert(hash_value, data);
    }

    // find
    {
      TestData data;
      const bool is_find = hash_table.find(hash_value, &data);
      ASSERT_TRUE(is_find);
      ASSERT_EQ(2, data.value);
    }  
  }

  void GetTableIndexTest(){
    HashTable<TestData> hash_table(test_table_space, kLockTable);
    const size_t table_size = HashTable<TestData>::CalcHashTableSize(test_table_space);

    constexpr size_t test_count = 10000;
    random_device seed_gen;
    mt19937_64 engine(seed_gen());

    for(size_t i=0; i<test_count; i++){
      const HashValue hash_value = engine();
      const auto index = hash_table.GetTableIndex(hash_value);
      const auto expected_index = hash_value % table_size;

      ASSERT_EQ(expected_index, index); 
    }
  }

  void LogicalInitializeTest()
  {
    HashTable<TestData> hash_table(test_table_space, kLockTable);
    constexpr HashValue hash_value = 1;
    
    // insert
    {
      TestData data;
      data.hash_value = hash_value;
      data.value = 1;
      hash_table.Upsert(hash_value, data);
    }

    // find
    {
      TestData data;
      const bool is_find = hash_table.find(hash_value, &data);
      ASSERT_TRUE(is_find);
      ASSERT_EQ(1, data.value);
    }

    hash_table.LogicalInitialize();

    {
      TestData data;
      const bool is_find = hash_table.find(hash_value, &data);
      ASSERT_FALSE(is_find);
    }
  }

  void LogicalInitializeLimitTest()
  {
    HashTable<TestData> hash_table(test_table_space, kLockTable);
    constexpr size_t kMaxCounter = numeric_limits<TableLogicCounter>::max();
    
    // todo upsertをしてtable要素数も試験する

    for(size_t i=2; i<=kMaxCounter; i++){
      hash_table.LogicalInitialize();
      ASSERT_EQ(i, hash_table.logic_counter_);
    }

    // kMaxCounterを超えると物理クリアされてカウンタは1に戻る
    hash_table.LogicalInitialize();
    ASSERT_EQ(1, hash_table.logic_counter_);
  }

  void CalcHashTableSizeTest(){
    {
      // 0MB -> 1,531要素(下限)
      constexpr size_t table_size = HashTable<uint64_t>::CalcHashTableSize(0);
      ASSERT_EQ(1531, table_size);
    }
    {
      // 1MB -> 98,299要素
      constexpr size_t table_size = HashTable<uint64_t>::CalcHashTableSize(test_table_space);
      ASSERT_EQ(49139, table_size);
    }
    {
      // 10MB -> 786,431要素
      constexpr size_t table_size = HashTable<uint64_t>::CalcHashTableSize(10 * test_table_space);
      ASSERT_EQ(393209, table_size);
    }
    {
      // 100MB -> 12,582,893要素
      constexpr size_t table_size = HashTable<uint64_t>::CalcHashTableSize(100 * test_table_space);
      ASSERT_EQ(6291449, table_size);
    }
    {
      // 1000MB -> 100,663,291要素
      constexpr size_t table_size = HashTable<uint64_t>::CalcHashTableSize(1000 * test_table_space);
      ASSERT_EQ(50331599, table_size);
    }
    {
      // 10000MB -> 805,306,357要素
      constexpr size_t table_size = HashTable<uint64_t>::CalcHashTableSize(10000 * test_table_space);
      ASSERT_EQ(402653117, table_size);
    }
    {
      // 100000MB -> 1610612711要素(上限)
      constexpr size_t table_size = HashTable<uint64_t>::CalcHashTableSize(100000 * test_table_space);
      ASSERT_EQ(1610612711, table_size);
    }
  }
};

TEST_F(HashTableTest, LockFreeConstructorTest)
{
  LockFreeConstructorTest();
}

TEST_F(HashTableTest, LockConstructorTest)
{
  LockConstructorTest();
}

TEST_F(HashTableTest, CalcHashTableSizeTest)
{
  CalcHashTableSizeTest();
}

TEST_F(HashTableTest, FindTest)
{
  FindTest();
}

TEST_F(HashTableTest, UpsertTest)
{
  UpsertTest();
}

TEST_F(HashTableTest, LogicalInitializeTest)
{
  LogicalInitializeTest();
}

TEST_F(HashTableTest, LogicalInitializeLimitTest)
{
  LogicalInitializeLimitTest();
}

TEST_F(HashTableTest, GetTableIndexTest)
{
  GetTableIndexTest();
}

}   // namespace realcore
