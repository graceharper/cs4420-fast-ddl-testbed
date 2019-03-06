#include<vector>
#include<iostream> 

int main() {
	
	//Creating the Database 2D array
	std::vector<std::vector<int>> stagDB; 
	
	//Creating and add a row to the DB
	std::vector<int> row(1,5);
	stagDB.push_back(row);
	stagDB[0].push_back(99); 
	stagDB[0].push_back(70); 
	stagDB[0].push_back(65); 
	stagDB[0].push_back(40); 
	stagDB[0].push_back(33); 
	
	//TODO : Creating and adding a row to the DB easier
	//int row_easy[5] = {0,2,3,4,5};
	//stagDB.insert(stagDB.end(), std::begin(row_easy), std::end(row_easy));

	//printing the first attribute from the first tuple in DB 	 
	std::cout << stagDB[0][0] << std::endl; 
	return 1; 
}
