/* Copyright (c) 2016-2016, Vasil Dimov

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include <iostream>
#include <stdexcept>
#include <string>

#include <ipfs/client.h>
#include <ipfs/test/utils.h>

int main(int, char**) {
  try {
    ipfs::Client client("localhost", 5001);

#if 0
    /** [ipfs::Client::DagPut] */
    ipfs::Json object_to_store = R"(
      {
          "Data": "another",
          "Links": [ {
              "Name": "some link",
              "Hash": "QmXg9Pp2ytZ14xgmQjYEiHjVjMFXzCVVEcRTWJBmLgR39V",
              "Size": 8
          } ]
      }
    )"_json;
    ipfs::Json object_stored;
    client.DagPut(object_to_store, &object_stored);
    std::cout << "Dag to store:" << std::endl
              << object_to_store.dump(2) << std::endl;
    std::cout << "Stored object:" << std::endl
              << object_stored.dump(2) << std::endl;
    /* An example output:
    Dag to store:
    {
      "Data": "another",
      "Links": [
        {
          "Hash": "QmXg9Pp2ytZ14xgmQjYEiHjVjMFXzCVVEcRTWJBmLgR39V",
          "Name": "some link",
          "Size": 8
        }
      ]
    }
    Stored object:
    {
      "Hash": "QmZZmY4KCu9r3e7M2Pcn46Fc5qbn6NpzaAGaYb22kbfTqm",
      "Links": [
        {
          "Hash": "QmXg9Pp2ytZ14xgmQjYEiHjVjMFXzCVVEcRTWJBmLgR39V",
          "Name": "some link",
          "Size": 8
        }
      ]
    }
    */
    /** [ipfs::Client::DagPut] */
    ipfs::test::check_if_properties_exist("client.DagPut()", object_stored,
                                          {"Hash", "Links"});

#endif

    /** [ipfs::Client::DagGet] */
    ipfs::Json object;
    client.DagGet("QmdfTbBqBPQ7VNxZEYEj14VmRuZBkqFbiwReogJgS1zR1n", &object);
    std::cout << "Dag: " << std::endl << object.dump(2) << std::endl;
    /* An example output:
    Dag:
    {
      "Data": "another",
      "Links": [ {
        "Name": "some link",
        "Hash": "QmXg9Pp2ytZ14xgmQjYEiHjVjMFXzCVVEcRTWJBmLgR39V",
        "Size": 8
      } ]
    }
    */
    /** [ipfs::Client::DagGet] */


#if 0
    /** [ipfs::Client::DagTree] */
    ipfs::Json stat;
    client.DagTree("QmYwAPJzv5CZsnA625s3Xf2nemtYgPpHdWEz79ojWnPbdG", &stat);
    std::cout << "Dag data size: " << stat["DataSize"] << std::endl;
    /* An example output:
    Dag data size: 2
    */
    /** [ipfs::Client::DagTree] */
    ipfs::test::check_if_properties_exist(
        "client.DagTree()", stat, {"BlockSize", "CumulativeSize", "DataSize",
                                      "Hash", "LinksSize", "NumLinks"});

    {
      /** [ipfs::Client::DagPatchAddLink] */
      /* Create a new node, upload two files and link them to the node. */

      std::string orig_id;
      client.DagNew(&orig_id);

      ipfs::Json added_files;
      client.FilesAdd(
          {{"file1.txt", ipfs::http::FileUpload::Type::kFileContents, "f1f1"},
           {"file2.txt", ipfs::http::FileUpload::Type::kFileContents, "f2f2"}},
          &added_files);

      std::string new_id;
      client.DagPatchAddLink(orig_id, "link to file1.txt",
                                added_files[0]["hash"], &new_id);

      ipfs::Json new_object;
      client.DagGet(new_id, &new_object);
      std::cout << "Added a link to " << orig_id << "." << std::endl
                << "New object " << new_id << ":" << std::endl
                << new_object.dump(2) << std::endl;
      /* An example output:
      Added a link to QmdfTbBqBPQ7VNxZEYEj14VmRuZBkqFbiwReogJgS1zR1n.
      New object QmRgn5ZeLiPRwQHqSbMLUZ2gkyLbJiaxVEx3LrDCqqaCAb:
      {
        "Data": "",
        "Links": [
          {
            "Hash": "QmNYaS23te5Rja36U94JoSTuMxJZmBEnHN8KEcjR6rGRGn",
            "Name": "link to file1.txt",
            "Size": 12
          }
        ]
      }
      */
#endif
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
