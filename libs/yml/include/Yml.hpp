#pragma once
/**
 * @file Yml.hpp
 * @author Gabriel Regidor Vallcanera
 * @brief Library to work with yml files through ryml
 * @version 0.1
 * @date 2022-10-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <ryml/ryml.hpp>
#include <ryml/ryml_std.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

class Yml {
    public:
        /**
         * @brief Construct a new Yml object
         * 
         * @param fileName: whatever file inside C:/QuixMind/QuixSmart
         */
        Yml(std::string filePath);
        /**
         * @brief Call this function every time you want reload file in case it was enternally modified
         * 
         */
        void LoadConfiguration(void);
        /**
         * @brief Get the Number Children of a ryml Node Ref
         * 
         */
        int GetNumberChildren(std::string rootName);
        /**
         * @brief Get bool parameter value
         * 
         */
        bool GetBool(std::string parameter);
        /**
         * @brief Get int parameter value
         * 
         */
        int GetInt(std::string parameter);
        /**
         * @brief Get double parameter value
         * 
         */
        double GetDouble(std::string parameter);
        /**
         * @brief Get std::std::string parameter value
         * 
         */
        std::string GetString(std::string parameter);
        /**
         * @brief Get std::vector<int> array value
         * 
         */
        std::vector<int> GetIntArray(std::string parameter);
        /**
         * @brief Get std::vector<double> array value
         * 
         */
        std::vector<double> GetDoubleArray(std::string parameter);
        /**
         * @brief Set bool parameter value
         * 
         */
        void SetBool(std::string parameter, bool value);
        /**
         * @brief Set Int parameter value
         * 
         */
        void SetInt(std::string parameter, int value);
        /**
         * @brief Set double parameter value
         * 
         */
        void SetDouble(std::string parameter, double value);
        /**
         * @brief Set std::string parameter value
         * 
         */
        void SetString(std::string parameter, std::string value);
        /**
         * @brief Set int array value
         * 
         */
        void SetIntArray(std::string parameter, std::vector<int> value);
        /**
         * @brief Set double array value
         * 
         */
        void SetDoubleArray(std::string parameter, std::vector<double> value);
        /**
         * @brief Save @TreeFile into @FilePath
         * 
         */
        void Save(void);

    private:
        ryml::NodeRef GetNode(std::string parameter, ryml::NodeRef* root);
        template <typename GetType> GetType RecursiveGet(std::string parameter, ryml::NodeRef root);
        template <typename GetType> std::vector<GetType> RecursiveGetArray(std::string parameter, ryml::NodeRef root);
        template <typename SetType> void RecursiveSet(std::string parameter, SetType value, ryml::NodeRef* root);
        template <typename SetType> void RecursiveSetArray(std::string parameter, std::vector<SetType> value, ryml::NodeRef* root);
        int RecursiveNumChildren(std::string parameter, ryml::NodeRef root);
        std::string FilePath;
        ryml::Tree TreeFile;
};