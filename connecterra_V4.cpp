//Algorithm with all the columns used.
//read_csv used 2D vector format to store all the data as string
//convert the read_data into required data type when needed

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility> 
#include <stdexcept> 
#include <sstream>
#include <algorithm>

void write_csv(std::string filename, std::vector<int> nodeID_vec, std::vector<double> gap2_vec, std::vector<double> overLap2_vec, std:: vector<double> gap3_vec, std::vector<double> overLap3_vec){
    // Make a CSV file with one or more columns of integer values
    // Each column of data is represented by each vector       
       
    // Create an output filestream object
    std::ofstream myFile(filename);
    
    // Enter column names to the stream
    myFile << "NodeID,gapTwo,overlapTwo,gapThree,overlapThree" << "\n";     
    
    // Send data to the stream
    //Run the loop for the size of the any vector
    for(int i = 0; i < nodeID_vec.size(); i++) 
    {
        myFile << nodeID_vec[i];
        myFile << ",";
        myFile << gap2_vec[i];
        myFile << ",";
        myFile << overLap2_vec[i];
        myFile << ",";
        myFile << gap3_vec[i];
        myFile << ",";
        myFile << overLap3_vec[i];        
        myFile << "\n";
    }
    
    // Close the file
    myFile.close();
}

std::vector<std::pair<std::string, std::vector<std::string>>> read_csv(std::string filename){
    // Reads a CSV file into a pair vector of <string, vector<string>> where
    // each pair represents <column name, column values(all the vlaues)>

    // Create a vector of <string, string vector> pairs to store the result
    std::vector<std::pair<std::string, std::vector<std::string>>> result;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line, colname, tmpStr;    

    // Read the column names
    if(myFile.good())
    {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while(std::getline(ss, colname, ',')){
            
            // Initialize and add <colname, string vector> pairs to result
            result.push_back({colname, std::vector<std::string> {}});
        }
    }

    // Read data, line by line
    while(std::getline(myFile, line))
    {
        // Create a stringstream of the current line
        std::stringstream ss(line); 
         int colIdx = 0;           

        // read each columns and save into invidual vector
        while(std::getline(ss, tmpStr, ',')){
            
            // Initialize and add <colname, int vector> pairs to result
            result.at(colIdx).second.push_back(tmpStr);
            colIdx++;
        }        
    }

    // Close file
    myFile.close();

    return result;
}

//find an element in vector and return its position
template < typename T>
std::pair<bool, int > findInVector(const std::vector<T>  & vecOfElements, const T  & element)
{
    std::pair<bool, int > result;
    // Find given element in vector
    auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);
    if (it != vecOfElements.end())
    {
        result.second = distance(vecOfElements.begin(), it);
        result.first = true;
    }
    else
    {
        result.first = false;
        result.second = -1;
    }
    return result;
}

int main(int argc, char *argv[]) {
    // Read three_cols.csv and ones.csv
    // std::vector<std::pair<std::string, std::vector<std::string>>> read_data = read_csv(argv[1]);//("test_input_programming_assignment.csv");    
    std::string filename{argv[1]};       
    std::vector<std::pair<std::string, std::vector<std::string>>> read_data = read_csv(filename);

    //Vectors to store data before wiriting into csv file
    std::vector<int> nodeID_vec;
    std::vector<double> gap2_vec;
    std::vector<double> overLap2_vec;
    std::vector<double> gap3_vec;
    std::vector<double> overLap3_vec;
    std::vector<double> endTime2_vec;
    std::vector<double> endTime3_vec;

    int nodeID;
    double epoch, duration, streamID;

    //Run the loop for the size of the dataset
    //i = rows from the csv file
    for(int i = 0; i < read_data.at(0).second.size(); ++i) 
    {   
        nodeID   = stoi(read_data.at(0).second.at(i));
        epoch    = stod(read_data.at(2).second.at(i));
        duration = stod(read_data.at(4).second.at(i));
        streamID = stoi(read_data.at(7).second.at(i));

        //checking if nodeID already exists in the vector, else insert it
        std::pair<bool, int> nodeID_result = findInVector<int>(nodeID_vec, nodeID);     
        int nodeID_pos = nodeID_result.second;


        if (nodeID_result.first)
        {
            if(streamID == 2)
            {   
                //if this is the first time, update endTime without gap or overlap comparision
                if(endTime2_vec.at(nodeID_pos) == -5)
                    endTime2_vec.at(nodeID_pos)= (epoch + duration);
                else
                {
                    //check for the Gap criteria
                    if(epoch >= endTime2_vec.at(nodeID_pos))
                        gap2_vec.at(nodeID_pos) += (epoch-endTime2_vec.at(nodeID_pos));

                    //check for overlap criteria
                    if (endTime2_vec.at(nodeID_pos) >= epoch)
                        overLap2_vec.at(nodeID_pos) += (endTime2_vec.at(nodeID_pos)-epoch);

                    endTime2_vec.at(nodeID_pos) = (epoch + duration);
                }
            }        

            if(streamID == 3)
            {   
                //if this is the first time, update endTime without gap or overlap comparision
                if(endTime3_vec.at(nodeID_pos) == -5)
                    endTime3_vec.at(nodeID_pos)= (epoch + duration);
                else
                {
                    if(epoch >= endTime3_vec.at(nodeID_pos))
                        gap3_vec.at(nodeID_pos) += (epoch-endTime3_vec.at(nodeID_pos));

                    //check for overlap criteria
                    if (endTime3_vec.at(nodeID_pos) >= epoch)
                        overLap3_vec.at(nodeID_pos) += (endTime3_vec.at(nodeID_pos)-epoch);

                    endTime3_vec.at(nodeID_pos) = (epoch + duration);
                }
            }   
        }
            
        else
        {            
            nodeID_vec.push_back(nodeID);
            gap2_vec.push_back(0);
            overLap2_vec.push_back(0);
            gap3_vec.push_back(0);
            overLap3_vec.push_back(0);

            //calculate endTime as epoch+duration based on streamID
            if(streamID == 2)                
                {
                    endTime2_vec.push_back(epoch + duration);
                    endTime3_vec.push_back(-5);
                }

            else if(streamID == 3)     
            {
                endTime2_vec.push_back(-5);
                endTime3_vec.push_back(epoch + duration);                                
            }
        }    
    }

    // Write to another file to check that this was successful
    write_csv("erroneous_data.csv", nodeID_vec,gap2_vec,overLap2_vec,gap3_vec,overLap3_vec);
    
    return 0;
}   