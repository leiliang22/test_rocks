#include <iostream>
#include <assert.h>
#include "rocksdb/db.h"

using std::cerr;
using std::endl;

int main() {
    rocksdb::DB *db;
    rocksdb::Options options;
    options.create_if_missing = true;

    // open a database with a name which corresponds to a file system directory
    rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb", &db);
    // check status
    if (!status.ok()) cerr << status.ToString() << endl;

    // Slice type can (should) be used in place of std::string for key-value
    rocksdb::Slice key = "foo";
    std::string value1 = "bar";

    // modify/query the database
    status = db->Put(rocksdb::WriteOptions(), key, value1);
    assert(status.ok());
    std::cout << status.ToString() << std::endl;

    std::string value2;
    status = db->Get(rocksdb::ReadOptions(), key, &value2);
    assert(status.ok() && value1 == value2);
    std::cout << value2 << " " << value1 << status.ToString() << std::endl;

    status = db->Delete(rocksdb::WriteOptions(), key);
    assert(status.ok());

    status = db->Get(rocksdb::ReadOptions(), key, &value2);
    assert(status.IsNotFound());


    // close the database
    delete db;

    return 0;
}
