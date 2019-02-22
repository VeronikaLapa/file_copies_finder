#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "finder.h"
#include <string>
#include <QTextStream>

#include <QDir>

using namespace testing;
QDir dir;
TEST(finder, EXAMPLE)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
}

TEST(finder, EMPTY_DIR)
{
    QDir test_dir(dir.path() +"/empty");
    if (!test_dir.exists()) {
         QDir().mkdir(dir.path() +"/empty");
    }
    finder f = finder(test_dir);
    f.find_copies();
    EXPECT_EQ(f.get_copies().size(), 0);
}

TEST(finder, TWO_COPIES)
{
    QDir test_dir(dir.path() +"/two_copies");
    if (!test_dir.exists()) {
         QDir().mkdir(dir.path() +"/two_copies");
    }
    for (int i = 0; i < 2; ++i) {
        QString filename = dir.path() + "/two_copies/" + QString::number(i) + ".txt";
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "abc" << endl;
        }
    }
    finder f = finder(test_dir);
    f.find_copies();
    EXPECT_EQ(f.get_copies().size(), 1);
    EXPECT_EQ(f.get_copies().begin()->size(), 2);
}

TEST(finder, CHOSEN_DIR)
{
    finder f = finder(QDir("D:/University/2017-2018/2017-2018_прога_весна/hw6_1/paradigms-2018"));
    f.find_copies();
    EXPECT_EQ(1, 1);
}

