#include <Yml.hpp>
#include <gtest/gtest.h>

TEST(YmlTest, GetData) {
    Yml* ymlTest = new Yml("C:/Users/Usuario/Desktop/YmlLibrary/config/example.yml");
    ymlTest->LoadConfiguration();
    EXPECT_EQ(ymlTest->GetInt("data_1"), 10);
    EXPECT_EQ(ymlTest->GetDouble("data_2"), 3.14);
    EXPECT_EQ(ymlTest->GetString("tree_1-subdata_4"), "bye");
}

TEST(YmlTest, SetData) {
    Yml* ymlTest = new Yml("C:/Users/Usuario/Desktop/YmlLibrary/config/example.yml");
    ymlTest->LoadConfiguration();   
    ymlTest->SetInt("tree_1-subdata_1", 11);
    ymlTest->SetBool("tree_1-subdata_2", false);
    ymlTest->SetDouble("tree_1-subdata_3", 0.8);
    ymlTest->SetString("data_4", "hi!");
    ymlTest->Save(); 
}