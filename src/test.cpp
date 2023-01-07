#include <catch.hpp>

#include "trie.h"
#include "canonical_codes.h"
#include "bin_io.h"
#include "priority_queue.h"

#include <sstream>
#include <ostream>

TEST_CASE("Priority_queue") {
    {
        auto get_min = [](const int &a, const int &b) {
            if (a < b) {
                return a;
            } else {
                return b;
            }
        };

        PriorityQueue<int, decltype(get_min)> q(get_min);

        q.Push(10);
        q.Push(22);
        REQUIRE(q.GetTop() == 10);
        q.Pop();
        REQUIRE(q.GetTop() == 22);
        q.Push(100);
        q.Push(101);
        REQUIRE(q.GetTop() == 22);
        REQUIRE(q.GetSize() == 3);
        q.Pop();
        q.Pop();
        REQUIRE(q.GetTop() == 101);
        REQUIRE(q.GetSize() == 1);
    }
    {
        auto get_min = [](const std::string &a, const std::string &b) {
            if (a > b) {
                return a;
            } else {
                return b;
            }
        };

        PriorityQueue<std::string, decltype(get_min)> q(get_min);

        q.Push("aaa");
        q.Push("aba");
        REQUIRE(q.GetTop() == "aba");
        q.Push("c");
        REQUIRE(q.GetTop() == "c");
        q.Pop();
        q.Pop();
        REQUIRE(q.GetSize() == 1);
        REQUIRE(q.GetTop() == "aaa");
    }
}

TEST_CASE("Canonical codes") {
    {
        std::vector<size_t> lengths = {1, 3, 3, 5};
        std::vector<std::vector<bool>> answer = {
            {false}, {true, false, false}, {true, false, true}, {true, true, false, false, false}};
        REQUIRE(GetCanonicalCodes(lengths) == answer);
    }
    {
        std::vector<size_t> length = {2, 3, 6};
        std::vector<std::vector<bool>> answer = {
            {false, false}, {false, true, false}, {false, true, true, false, false, false}};
    }
}

TEST_CASE("Binary output") {
    {
        std::stringstream stream1;
        std::ostream &stream2(static_cast<std::ostream &>(stream1));
        Writer writer(stream2);
        writer.PutWord({false, false, true, true, false, false, false, false});
        writer.PutWord({false, false, true, true, false, false, false, true});
        writer.Flush();
        REQUIRE(stream1.str() == "01");
    }
    {
        std::stringstream stream1;
        std::ostream &stream2(static_cast<std::ostream &>(stream1));
        Writer writer(stream2);
        writer.PutBit(false);
        writer.PutBit(false);
        writer.PutBit(true);
        writer.PutBit(true);
        writer.Flush();
        REQUIRE(stream1.str() == "0");
    }
}

TEST_CASE("Binary input") {
    {
        std::stringstream stream1;
        stream1 << "az";
        std::istream &stream2(static_cast<std::istream &>(stream1));
        Reader reader(stream2);
        REQUIRE(reader.GetBit() == false);
        REQUIRE(reader.GetChar() == 194);
    }
    {
        std::stringstream stream1;
        stream1 << "0";
        std::istream &stream2(static_cast<std::istream &>(stream1));
        Reader reader(stream2);
        REQUIRE(reader.GetBit() == false);
        REQUIRE(reader.GetBit() == false);
        REQUIRE(reader.GetBit() == true);
        REQUIRE(reader.GetBit() == true);
    }
}

TEST_CASE("Trie") {
    {
        Trie t1(50);
        Trie t2(60);
        Trie t3 = Trie::Merge(t1, t2);
        std::vector<std::pair<int, size_t>> answer = {{50, 1}, {60, 1}};
        REQUIRE(t3.GetCodes() == answer);
    }
    {
        Trie t1(50);
        Trie t2(60);
        Trie t3 = Trie::Merge(t1, t2);
        t3.CheckStart();
        REQUIRE(t3.CheckAdd(false));
    }
    {
        Trie t1 = Trie(0);
        Trie t2 = Trie(1);
        Trie t3 = Trie(2);
        Trie t4 = Trie::Merge(t1, t2);
        Trie t = Trie::Merge(t4, t3);
        t.CheckStart();
        REQUIRE(!t.CheckAdd(false));
        t.CheckStart();
        REQUIRE(t.CheckAdd(true));
    }
}
