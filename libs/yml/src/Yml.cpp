#include <Yml.hpp>
#include <typeinfo>

Yml::Yml(std::string filePath) : FilePath(filePath) {
    char* filePath_c = &FilePath[0];
    std::ifstream inputFile(filePath_c, std::ios::in | std::ios::binary);
    if (!inputFile.is_open()) {
        std::cout<<"Could not open "<<FilePath<<std::endl;
        exit(EXIT_FAILURE);
    }
}

void Yml::LoadConfiguration(void) {
    char* filePath_c = &FilePath[0];
    std::ifstream inputFile(filePath_c, std::ios::in | std::ios::binary);
    std::ostringstream contents;
    contents << inputFile.rdbuf();
    std::string stringContents = contents.str();
    TreeFile = ryml::parse(ryml::to_csubstr(stringContents));
}

int Yml::GetNumberChildren(std::string rootName) {
    int numChildren = 0;
    if(!rootName.compare("")) numChildren = (int)TreeFile.rootref().num_children();
    else numChildren = RecursiveNumChildren(rootName, TreeFile.rootref());
    return numChildren;
}

bool Yml::GetBool(std::string parameter) {
    bool data = RecursiveGet<bool>(parameter, TreeFile.rootref());
    return data;
}

int Yml::GetInt(std::string parameter) {
    int data = RecursiveGet<int>(parameter, TreeFile.rootref());
    return data;
}

double Yml::GetDouble(std::string parameter) {
    double data = RecursiveGet<double>(parameter, TreeFile.rootref());
    return data;
}

std::string Yml::GetString(std::string parameter) {
    std::string data = RecursiveGet<std::string>(parameter, TreeFile.rootref());
    return data;
}

std::vector<int> Yml::GetIntArray(std::string parameter) {
    std::vector<int> data = RecursiveGetArray<int>(parameter, TreeFile.rootref());
    return data;
}

std::vector<double> Yml::GetDoubleArray(std::string parameter) {
    std::vector<double> data = RecursiveGetArray<double>(parameter, TreeFile.rootref());
    return data;
}

void Yml::SetBool(std::string parameter, bool value) {
    RecursiveSet<bool>(parameter, value, &TreeFile.rootref());
}

void Yml::SetInt(std::string parameter, int value) {
    RecursiveSet<int>(parameter, value, &TreeFile.rootref());
} 

void Yml::SetDouble(std::string parameter, double value) {
    RecursiveSet<double>(parameter, value, &TreeFile.rootref());
}

void Yml::SetString(std::string parameter, std::string value) {
    RecursiveSet<std::string>(parameter, value, &TreeFile.rootref());
}

void Yml::SetIntArray(std::string parameter, std::vector<int> value) {
    RecursiveSetArray<int>(parameter, value, &TreeFile.rootref());
}

void Yml::SetDoubleArray(std::string parameter, std::vector<double> value) {
    RecursiveSetArray<double>(parameter, value, &TreeFile.rootref());
}

void Yml::Save(void) {
    char* filePath_c = &FilePath[0];
    FILE* file = fopen(filePath_c, "w");   
    ryml::emit(TreeFile.rootref(), file);
    fclose (file);
}

ryml::NodeRef Yml::GetNode(std::string parameter, ryml::NodeRef* root) {
    ryml::NodeRef data;
    for(int i=0; i<root->num_children(); i++){
        if((*root)[i].key() == parameter){
            memcpy(&data, &((*root)[i]), sizeof(ryml::NodeRef));
            break;
        }
    }
    return data;
}

template <typename GetType> GetType Yml::RecursiveGet(std::string parameter, ryml::NodeRef root) {
    size_t delimiterPos = parameter.find("-");
    if(delimiterPos!=std::string::npos){
        ryml::NodeRef childRoot = GetNode(parameter.substr(0, delimiterPos), &root);
        return RecursiveGet<GetType>(parameter.substr(delimiterPos+1, parameter.size()), childRoot);
    }
    else{
        c4::substr param_c4 = c4::substr(&parameter[0], parameter.size());
        if(!root[param_c4].has_val()){
            std::cout<<"El parametro '"<<parameter<<"' no se encuentra en el fichero "<<FilePath<<std::endl;
            exit(0);
        }
        GetType data;
        root[param_c4] >> data;
        return data;  
    }
}

template <typename GetType> std::vector<GetType> Yml::RecursiveGetArray(std::string parameter, ryml::NodeRef root) {
    size_t delimiterPos = parameter.find("-");
    if(delimiterPos!=std::string::npos){
        ryml::NodeRef childRoot = GetNode(parameter.substr(0, delimiterPos), &root);
        return RecursiveGetArray<GetType>(parameter.substr(delimiterPos+1, parameter.size()), childRoot);
    }
    else{
        c4::substr param_c4 = c4::substr(&parameter[0], parameter.size());
        if(!root[param_c4][0].has_val()){
            std::cout<<"El parametro '"<<parameter<<"' no se encuentra en el fichero "<<FilePath<<std::endl;
            exit(0);
        }
        std::vector<GetType> data;
        int idx = 0;
        while(1){
            GetType val;
            root[param_c4][idx] >> val;
            data.push_back(val);
            idx++;
            if(!root[param_c4][idx].has_val()) break;
        }
        return data;  
    }
}

template <typename SetType> void Yml::RecursiveSet(std::string parameter, SetType value, ryml::NodeRef* root) {
    size_t delimiterPos = parameter.find("-");
    if(delimiterPos!=std::string::npos){
        ryml::NodeRef childRoot = GetNode(parameter.substr(0, delimiterPos), root);
        RecursiveSet<SetType>(parameter.substr(delimiterPos+1, parameter.size()), value, &childRoot);
    }
    else{
        c4::substr param_c4 = c4::substr(&parameter[0], parameter.size());
        (*root)[param_c4] << value;    
    }
}

template <typename SetType> void Yml::RecursiveSetArray(std::string parameter, std::vector<SetType> value, ryml::NodeRef* root) {
    size_t delimiterPos = parameter.find("-");
    if(delimiterPos!=std::string::npos){
        ryml::NodeRef childRoot = GetNode(parameter.substr(0, delimiterPos), root);
        RecursiveSetArray<SetType>(parameter.substr(delimiterPos+1, parameter.size()), value, &childRoot);
    }
    else{
        c4::substr param_c4 = c4::substr(&parameter[0], parameter.size());
        for(int i=0; i<value.size(); i++){
            SetType val = value[i];
            (*root)[param_c4][i] << val;
        }
    }
}

int Yml::RecursiveNumChildren(std::string parameter, ryml::NodeRef root) {
    size_t delimiterPos = parameter.find("-");
    if(delimiterPos!=std::string::npos){
        ryml::NodeRef childRoot = GetNode(parameter.substr(0, delimiterPos), &root);
        return RecursiveNumChildren(parameter.substr(delimiterPos+1, parameter.size()), childRoot);
    }
    else{
        ryml::NodeRef childRoot = GetNode(parameter, &root);
        return (int)childRoot.num_children();    
    }
}

