class Movie {
    // Define properties and methods for movie class
public:
    string title;
    string genre;
    int duration;
    string classification;
    string synopsis;
    string trailer;
    vector<string> showtimes;
};

struct DecisionNode {
    std::string question;
    std::string genre;
    std::vector<Movie> movies;
    std::unique_ptr<DecisionNode> yesChild;
    std::unique_ptr<DecisionNode> noChild;
};

std::unique_ptr<DecisionNode> buildDecisionTree(const std::vector<Movie>& movies) {
    // Base case: no movies to suggest
    if (movies.empty()) {
        return nullptr;
    }

    // Base case: all movies have the same genre
    std::string genre = movies.front().genre;
    if (std::all_of(movies.begin() + 1, movies.end(), [&](const Movie& m) { return m.genre == genre; })) {
        auto node = std::make_unique<DecisionNode>();
        node->question = "Do you want to watch a " + genre + " movie?";
        node->genre = genre;
        node->movies = movies;
        return node;
    }

    // Recursive case: find the best question to ask
    std::string bestQuestion;
    int bestQuestionScore = 0;
    std::vector<Movie> bestYesMovies, bestNoMovies;
    for (const auto& movie : movies) {
        std::vector<Movie> yesMovies, noMovies;
        for (const auto& otherMovie : movies) {
            if (otherMovie.genre == movie.genre) {
                yesMovies.push_back(otherMovie);
            }
            else {
                noMovies.push_back(otherMovie);
            }
        }
        int score = std::min(yesMovies.size(), noMovies.size());
        if (score > bestQuestionScore) {
            bestQuestionScore = score;
            bestQuestion = "Do you want to watch a " + movie.genre + " movie?";
            bestYesMovies = yesMovies;
            bestNoMovies = noMovies;
        }
    }


    // Create decision node and recursively build subtrees
    auto node = std::make_unique<DecisionNode>();
    node->question = bestQuestion;
    node->genre = "";
    node->movies = {};
    node->yesChild = buildDecisionTree(bestYesMovies);
    node->noChild = buildDecisionTree(bestNoMovies);
    return node;
}


class Showtime {
    // Define properties and methods for showtime class
private:
    string movieTitle;
    string cinemaHall;
    string startDateTime;
    string endDateTime;
    float price;

public:
    Showtime(string movieTitle, string cinemaHall, string startDateTime, string endDateTime, float price)
        : movieTitle(movieTitle), cinemaHall(cinemaHall), startDateTime(startDateTime), endDateTime(endDateTime), price(price) {}

    // Getter methods
    string getMovieTitle() const {
        return movieTitle;
    }

    string getCinemaHall() const {
        return cinemaHall;
    }

    string getStartDateTime() const {
        return startDateTime;
    }

    string getEndDateTime() const {
        return endDateTime;
    }

    float getPrice() const {
        return price;
    }

    // Setter methods
    void setMovieTitle(string title) {
        movieTitle = title;
    }

    void setCinemaHall(string hall) {
        cinemaHall = hall;
    }

    void setStartDateTime(string start) {
        startDateTime = start;
    }

    void setEndDateTime(string end) {
        endDateTime = end;
    }

    void setPrice(float newPrice) {
        price = newPrice;
    }

    // Display method
    void display() const {
        cout << "Movie title: " << movieTitle << endl;
        cout << "Cinema hall: " << cinemaHall << endl;
        cout << "Start time: " << startDateTime << endl;
        cout << "End time: " << endDateTime << endl;
        cout << "Price: $" << price << endl;

    };
};

class CinemaHall {
    // Define properties and methods for cinema hall class
public:
    CinemaHall(int hallNumber, int capacity) : hallNumber(hallNumber), capacity(capacity) {}

    int getHallNumber() const {
        return hallNumber;
    }

    int getCapacity() const {
        return capacity;
    }

    void setCapacity(int newCapacity) {
        capacity = newCapacity;
    }

private:
    int hallNumber;
    int capacity;
};

class Reservation {
    // Define properties and methods for reservation class
public:
    string movieTitle;
    string cinemaHall;
    int showtime;
    int numTickets;
};

// Implement functions for searching, reserving, and purchasing tickets

void searchMovies(vector<Movie>& movies) {
    // Implement search function for available movies
    string searchCriteria;
    cout << "Please enter your search criteria: ";
    cin >> searchCriteria;

    // Filter movies based on search criteria
    vector<Movie> filteredMovies;
    for (const auto& movie : movies) {
        if (movie.title.find(searchCriteria) != string::npos ||
            movie.genre.find(searchCriteria) != string::npos ||
            movie.classification.find(searchCriteria) != string::npos) {
            filteredMovies.push_back(movie);
        }
    }

    // Display filtered movies
    if (filteredMovies.empty()) {
        cout << "No movies found. Please try again." << endl;
    }
    else {
        cout << "Available movies:" << endl;
        for (const auto& movie : filteredMovies) {
            cout << "- " << movie.title << " (" << movie.duration << " min, " << movie.classification << ")" << endl;
        }
    }
}

void reserveTicket(queue<Reservation>& reservations, vector<Movie>& movies) {
    // Implement ticket reservation process
    string movieTitle;
    cout << "Please enter the title of the movie you want to reserve: ";
    getline(cin, movieTitle);

    // Find movie by title
    auto movieIt = find_if(movies.begin(), movies.end(), [&](const Movie& movie) {
        return movie.title == movieTitle;
        });

    if (movieIt == movies.end()) {
        cout << "Movie not found. Please try again." << endl;
        return;
    }

    // Display available showtimes
    cout << "Available showtimes for " << movieTitle << ":" << endl;
    for (int i = 0; i < movieIt->showtimes.size(); i++) {
        cout << "- Showtime " << i + 1 << ": " << movieIt->showtimes[i] << endl;
    }

    // Prompt user to choose showtime and number of tickets
    string cinemaHall;
    int showtime;
    int numTickets;
    cout << "Please enter the cinema hall: ";
    getline(cin, cinemaHall);
    cout << "Please enter the showtime (1-" << movieIt->showtimes.size() << "): ";
    cin >> showtime;
    cout << "Please enter the number of tickets: ";
    cin >> numTickets;

    // Add reservation to queue
    Reservation reservation;
    reservation.movieTitle = movieTitle;
    reservation.cinemaHall = cinemaHall;
    reservation.showtime = showtime;
    reservation.numTickets = numTickets;
    reservations.push(reservation);
}

void purchaseTicket(const vector<Movie>& movies) {
    // Implement ticket purchase process
    string movieTitle;
    cout << "Please enter the title of the movie you want to purchase: ";
    getline(cin, movieTitle);// Find movie by title
    auto movieIt = find_if(movies.begin(), movies.end(), [&](const Movie& movie) {
        return movie.title == movieTitle;
        });

    if (movieIt == movies.end()) {
        cout << "Movie not found. Please try again." << endl;
        return;
    }

    // Display available showtimes
    cout << "Available showtimes for " << movieTitle << ":" << endl;
    for (int i = 0; i < movieIt->showtimes.size(); i++) {
        cout << "- Showtime " << i + 1 << ": " << movieIt->showtimes[i] << endl;
    }

    // Prompt user to choose showtime and number of tickets
    string cinemaHall;
    int showtime;
    int numTickets;
    cout << "Please enter the cinema hall: ";
    getline(cin, cinemaHall);
    cout << "Please enter the showtime (1-" << movieIt->showtimes.size() << "): ";
    cin >> showtime;
    cout << "Please enter the number of tickets: ";
    cin >> numTickets;

    // Calculate total cost of tickets
    float ticketPrice = 10.0f; // Assuming a fixed ticket price of $10
    float totalPrice = ticketPrice * numTickets;

    // Prompt user to pay for tickets
    cout << "Total cost: $" << totalPrice << endl;
    string paymentMethod;
    cout << "Please choose a payment method (credit card, debit card, cash): ";
    getline(cin, paymentMethod);

    cout << "Purchase successful. Enjoy the movie!" << endl;
}



// Implement additional features



void recommendationSystem(vector<Movie>& movies) {
    // Implement recommendation system
    // Build decision tree
    auto root = buildDecisionTree(movies);
    // Traverse decision tree to suggest movies based on user preferences
    auto currentNode = root.get();
    while (currentNode->movies.empty()) {
        cout << currentNode->question << " (y/n): ";
        char answer;
        cin >> answer;
        if (answer == 'y') {
            currentNode = currentNode->yesChild.get();
        }
        else {
            currentNode = currentNode->noChild.get();
        }
    }

    // Print suggested movies
    cout << "Here are some movies you might like:" << endl;
    for (const auto& movie : currentNode->movies) {
        cout << movie.title << " (" << movie.genre << ")" << endl;
    }

}

void loyaltyPoints(unordered_map<string, int>& points) {
    // Implement loyalty point system

}
void awardPoints(unordered_map<string, int>& points) {
    string username;
    cout << "Please enter your username: ";
    getline(cin, username);

    // Find user by username
    auto it = points.find(username);
    if (it == points.end()) {
        // User not found, create new entry
        points[username] = 1;
        cout << "Congratulations, you have earned 1 loyalty point!" << endl;
    }
    else {
        // User found, increment points
        ++it->second;
        cout << "Congratulations, you have earned another loyalty point! You now have " << it->second << " points." << endl;
    }
}
// Implement a function to check loyalty points for a user
void checkPoints(const unordered_map<string, int>& points) {
    string username;
    cout << "Please enter your username: ";
    getline(cin, username);

    // Find user by username
    auto it = points.find(username);
    if (it == points.end()) {
        cout << "User not found. Please try again." << endl;
        return;
    }

    cout << "You have " << it->second << " loyalty points." << endl;
}

// Implement a function to use loyalty points to get a discount on ticket prices
void usePoints(unordered_map<string, int>& points, vector<Movie>& movies, array<vector<Showtime>, 5>& showtimes) {
    string username;

    cout << "Please enter your username: ";
    getline(cin, username);

    // Find user by username
    auto it = points.find(username);
    if (it == points.end()) {
        cout << "User not found. Please try again." << endl;
        return;
    }

    // Check if user has enough points to get a discount
    if (it->second < 10) {
        cout << "You do not have enough points to get a discount. Please earn more points and try again." << endl;
        return;
    }

    // Prompt user to select a movie and showtime
    int movieIndex;
    int showtimeIndex;
    cout << "Please select a movie:" << endl;
    for (int i = 0; i < movies.size(); ++i) {
        cout << i + 1 << ". " << movies[i].title << endl;
    }
    cin >> movieIndex;
    --movieIndex; // Adjust for 0-based indexing
    if (movieIndex < 0 || movieIndex >= movies.size()) {
        cout << "Invalid movie selection. Please try again." << endl;
        return;
    }
    cout << "Please select a showtime:" << endl;
    for (int i = 0; i < showtimes[movieIndex].size(); ++i) {
        cout << i + 1 << ". " << showtimes[movieIndex][i].getStartDateTime() << endl;
    }
    cin >> showtimeIndex;
    --showtimeIndex; // Adjust for 0-based indexing
    if (showtimeIndex < 0 || showtimeIndex >= showtimes[movieIndex].size()) {
        cout << "Invalid showtime selection. Please try again." << endl;
        return;
    }

    // Calculate discounted price
    float originalPrice = showtimes[movieIndex][showtimeIndex].getPrice();
    float discountedPrice = originalPrice * 0.9; // 10% discount for 10 or more loyalty points

    // Update user points and showtime price

    cout << "Congratulations, you have received a 10% discount on your ticket price! Your new ticket price is $" << discountedPrice << "." << endl;
}



void employeeManagement() {
    // Implement employee management features
}

void attendanceMonitoring(unordered_map<string, int>& attendance, unordered_map<string, float>& ticketPrices) {
    // Implement attendance monitoring feature
}

void priceReduction() {
    // Implement price reduction feature
}
// Define a class for employees
class Employee {
public:
    string name;
    string position;
    float salary;
    // Add any other necessary properties

    // Define a constructor for the employee class
    Employee(string name, string position, float salary) : name(name), position(position), salary(salary) {}

    // Define any necessary methods for the employee class
};

// Define a class for the cinema
class Cinema {
private:
    vector<Employee> employees;
    // Add any other necessary properties

public:
    // Define methods for managing employees
    void addEmployee(Employee employee) {
        employees.push_back(employee);
    }

    void removeEmployee(string name) {
        employees.erase(remove_if(employees.begin(), employees.end(), [&name](const Employee& employee) { return employee.name == name; }), employees.end());
    }

    void updateEmployeePosition(string name, string newPosition) {
        auto it = find_if(employees.begin(), employees.end(), [&name](const Employee& employee) { return employee.name == name; });
        if (it != employees.end()) {
            it->position = newPosition;
        }
    }

    void updateEmployeeSalary(string name, float newSalary) {
        auto it = find_if(employees.begin(), employees.end(), [&name](const Employee& employee) { return employee.name == name; });
        if (it != employees.end()) {
            it->salary = newSalary;
        }
    }

    // Add any other necessary methods
};
void mainMenu() {
    vector<Movie> movies;
    vector<vector<Showtime>> showtimes;
    queue<Reservation> reservations;
    unordered_map<string, vector<int>> ratings;
    unordered_map<string, vector<string>> comments;
    vector<CinemaHall> cinemaHalls;
    unordered_map<string, int> points;
    unordered_map<string, int> attendance;
    unordered_map<string, float> ticketPrices;

    int choice;
    do {
        cout << "Please select an option:" << endl;
        cout << "1. Search for available movies" << endl;
        cout << "2. Reserve a ticket" << endl;
        cout << "3. Purchase a ticket" << endl;
        cout << "4. Get movie recommendations" << endl;
        cout << "5. Check loyalty points" << endl;
        cout << "6. Manage employees" << endl;
        cout << "7. Monitor attendance and adjust ticket prices" << endl;
        cout << "8. Apply price reduction for multiple tickets" << endl;
        cout << "0. Quit" << endl;

        cin >> choice;

        switch (choice) {
        case 1:
            searchMovies(movies);
            break;
        case 2:
            reserveTicket(reservations, movies);
            break;
        case 3:
            purchaseTicket(movies);
            break;
        case 4:
            recommendationSystem(movies);
            break;
        case 5:
            loyaltyPoints(points);
            break;
        case 6:
            employeeManagement();
            break;
        case 7:
            attendanceMonitoring(attendance, ticketPrices);
            break;
        case 8:
            priceReduction();
            break;
        case 0:
            cout << "Thank you for using the cinema ticket reservation system. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }

    } while (choice != 0);
}


// Usage example
int main()
{
    //Display Main Menu 
    mainMenu();
    // Create a cinema object
    Cinema cinema;

    // Add employees
    cinema.addEmployee(Employee("John Doe", "Ticket Sales", 2000.0));
    cinema.addEmployee(Employee("Jane Smith", "Concessions", 1500.0));
    cinema.addEmployee(Employee("Bob Johnson", "Security", 2500.0));

    // Remove an employee
    cinema.removeEmployee("Jane Smith");

    // Update an employee's position
    cinema.updateEmployeePosition("John Doe", "Manager");

    // Update an employee's salary
    cinema.updateEmployeeSalary("Bob Johnson", 3000.0);

    // Add any other necessary code
}