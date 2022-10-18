//2-7-2022
//This program reads a list of movie data from a file, and stores
//it in a database. This data can then be accessed by the user searching
//a movie and the program finding and displays its data if in the database.
//The program also will save movie data that the user wants and display it
//at the end.

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

struct Movie {
private:
    string title;
    double grossTotal;
    string director;
    string releaseDate;
    int runtime;

public:

    void setTitle(string Title)
    {title = Title;}
    void setGrossTotal(double gTotal)
    {grossTotal = gTotal;}
    void setDirector(string direct)
    {director = direct;}
    void setReleaseDate(string releaseD)
    {releaseDate = releaseD;}
    void setRuntime(int runT)
    {runtime = runT;}

    string getTitle() const
    {return title ;}
    double getGrossTotal() const
    {return grossTotal;}
    string getDirector() const
    {return director;}
    string getReleaseDate() const
    {return releaseDate;}
    int getRuntime() const
    {return runtime;}
};

int numberOfLines(ifstream&);
void populateMovieFromFile(ifstream&, Movie&);
void displayMovie(const Movie&);
bool caseInsensitiveCmp(string, string);
void findMovie(Movie*, int);
void saveToFile(const Movie&);
bool promptToContinue();
void displayFavorites();
bool caseInsensitiveCmp(string, string);
Movie* createDatabase(int&);

int main() {
    int lines = 0;

    Movie* database = createDatabase(lines);

    do {
        findMovie(database, lines);
    } while (promptToContinue());

    displayFavorites();
    delete[] database;

    return 0;
}

//This function receives the number of lines in the text file
//and creates an array of objects in the heap containing the
//data in the file. It then returns a pointer to the array.
Movie* createDatabase(int& numOfLines){
    string fileName;
    ifstream inFile;

    do {
        cout << "Please enter filename: ";
        getline(cin, fileName);

        inFile.open(fileName);

        if (inFile) {
            numOfLines = numberOfLines(inFile);
            inFile.clear();
            inFile.seekg(0);

            Movie* moviePtr = new Movie[numOfLines];

            for(int i = 0; i < numOfLines; i++) {
                populateMovieFromFile(inFile, moviePtr[i]);
            }

            inFile.close();

            return moviePtr;
        }
        else {
            cout << "File was not found or does not exist" << endl;
        }

    } while (!inFile);
}

//This function receives the open file, reads the file to
//find how many lines there are in the file, and then returns
//that number of lines.
int numberOfLines(ifstream& inFile){
    int numOfLines = 0;
    string info;
    while(getline(inFile, info)){
        numOfLines++;
    }
    return numOfLines;
}

//This function receives the open file and a specific object array
//element and inputs the file data into that array struct. Does not
//return anything.
void populateMovieFromFile(ifstream& inFile, Movie& movieArr){
    string sTitle, sGross, sDirector, sReleaseD, sRuntime;

    getline(inFile, sTitle, ',');
    getline(inFile, sGross, ',');
    getline(inFile, sDirector, ',');
    getline(inFile, sReleaseD, ',');
    getline(inFile, sRuntime);

    movieArr.setTitle(sTitle);
    movieArr.setGrossTotal(stod(sGross));
    movieArr.setDirector(sDirector);
    movieArr.setReleaseDate(sReleaseD);
    movieArr.setRuntime(stoi(sRuntime));

}

//This function receives pointer to object array and number of lines
//in the file and searches if the movie inputted is in the file.
//If the movie is in the file is displays it data and saves it if
//the user wants to in a file. Returns nothing.
void findMovie(Movie* moviePtr, int numOfLines){
    string searchMovie;
    char save;
    bool found = false;
    int index = 0;
    //I do not know how to use this pointer

    cout << "\nEnter a movie title to search for: ";
    getline(cin, searchMovie);
    cout << endl;

    for (int i = 0; i < numOfLines; i++){
        if (caseInsensitiveCmp(searchMovie, moviePtr[i].getTitle())){
            found = true;
            index = i;
            break;
        }
        else
            found = false;
    }

    if (found) {

        //pointer or const arr and how to pass
        displayMovie(moviePtr[index]);

        cout << "Would you like to save the movie above? (Y / N): ";
        cin >> save;
        if (save == 'Y' || save == 'y') {
            saveToFile(moviePtr[index]);
        }
    }
    else {
        cout << setw(15) << "Error: " << "'" << searchMovie
             << "' not found in database. "
                "Please try again.\n" << endl;
    }

}

//This function receives the searched movie and a movie from
//the file and compares them, not dependent on capitalization.
//The function then returns if the movie is found or not.
bool caseInsensitiveCmp(string search, string movie){
    if(search.length() != movie.length())
        return false;
    for(int i = 0; i < movie.length(); i++){
        if(tolower(search[i]) != tolower(movie[i]))
            return false;
    }
    return true;
}

//This function receives a specific element/movie from the object
//array and displays its data. Does not return anything.
void displayMovie(const Movie& movieArr){
    cout << setw(15) << "Title: " << movieArr.getTitle() << endl;
    cout << setw(15) << "Gross Total: " << movieArr.getGrossTotal() << endl;
    cout << setw(14) << "Director:" << movieArr.getDirector() << endl;
    cout << setw(14) << "Release Date:" << movieArr.getReleaseDate() << endl;
    cout << setw(15) << "Runtime: " << movieArr.getRuntime() << endl;
    cout << endl;
}

//This function receives a specific element/movie from the object
//array and saves its data in a file that either exists
//or is then created. Does not return anything.
void saveToFile(const Movie& movie) {
    ofstream outFile;
    outFile.open("favorites.txt", ios::app);

    outFile << setw(15) << "Title: " << movie.getTitle() << endl;
    outFile << setw(15) << "Gross Total: " << movie.getGrossTotal() << endl;
    outFile << setw(14) << "Director:" << movie.getDirector() << endl;
    outFile << setw(14) << "Release Date:" << movie.getReleaseDate() << endl;
    outFile << setw(15) << "Runtime: " << movie.getRuntime() << endl;
    outFile << endl;

    cout << "Successfully saved to favorites.txt\n" << endl;
    outFile.close();
}

//This function receives nothing and asks the user if they would
//like to exit the search program and returns their
//decision in true or false.
bool promptToContinue(){
    char exit;
    bool repeat = false;
    do {
        if(repeat == true){
            cout << "Please enter either 'Y' or 'N'" << endl;
        }
        cout << "Would you like to exit? (Y / N): ";
        cin >> exit;
        cin.ignore();
        if (exit == 'N' || exit == 'n') {
            return true;
        }
        else if (exit == 'Y' || exit == 'y') {
            return false;
        }
        else
            repeat = true;

    } while(repeat);
}

//This function receives nothing and displays the movies that the
//user decided to save, at the end of the program. Returns nothing.
void displayFavorites(){
    ifstream inFile;
    inFile.open("favorites.txt");

    int saved = 0;
    if(inFile){
        string info;
        cout << "\nYour saved movies are: \n" << endl;
        while(getline(inFile, info)){
            cout << info << endl;
            saved++;
        }
        if(saved == 2)
            cout << setw(15) << "Error: " << "No movies saved" << endl;
    }
    else {
        cout << setw(15) << "Error: " << "No movies saved" << endl;
    }

    inFile.close();
}

/*
Please enter filename: save.txt
File was not found or does not exist
Please enter filename: movie.txt

Enter a movie title to search for: MINionS

        Title: Minions
  Gross Total: 1.159
     Director: Pierre Coffin & Kyle Blada
 Release Date: 7/10/15
      Runtime: 91

Would you like to save the movie above? (Y / N): Y
Successfully saved to favorites.txt

Would you like to exit? (Y / N): n

Enter a movie title to search for: SeaFall

        Error: 'SeaFall' not found in database. Please try again.

Would you like to exit? (Y / N): N

Enter a movie title to search for: SKYfall

        Title: Skyfall
  Gross Total: 1.109
     Director: Sam Mendes
 Release Date: 11/9/12
      Runtime: 143

Would you like to save the movie above? (Y / N): n
Would you like to exit? (Y / N): y

Your saved movies are:

        Title: Minions
  Gross Total:  1.159
     Director: Pierre Coffin & Kyle Blada
 Release Date: 7/10/15
      Runtime:  91


Process finished with exit code 0
 */
