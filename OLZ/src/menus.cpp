#include "menus.h"

#include"class/menu/menu.h"
#include"class/advertisement/advertisement.h"
#include"class/advertisement/sale/sale.h"
#include"class/advertisement/purchase/purchase.h"
#include"class/data/data.h"
#include"class/menu/searchMenu/searchMenu.h"
#include<iostream>
#include<stdlib.h>
#include"class/menu/optionMenu/optionMenu.h"

/*Menu Presentation
 *
 * 1 - Search
 * 2 - Sign In
 * 3 - Sign Up
 * 4 - Exit
 */

void clearScreen(){
	for(unsigned int i = 0; i < 50; i++)
		cout << endl;
}

void mainMenu(Data* data){
	clearScreen();
	OptionMenu menu(data);
	menu.addOption("Search", &search);
	menu.addOption("Sign In", &signIn);
	menu.addOption("Sign Up", &signUp);
	menu.addOption("Exit", &exitApp);
	menu.createMenu();
}

void search(Data* data){
	clearScreen();
	string search;
	cout << "\nInsert what you would like to search for: ";
	getline(cin, search);
	vector<Advertisement* > results;
	results = data->searchForAds(search);
	SearchMenu menu(data, results);
	menu.createMenu();
	if(data->getSignedInUser() != NULL)
		signedInMenu(data);
	else
		mainMenu(data);
}

void signIn(Data* data){
	clearScreen();
	string email, password;

	cout << "Email: ";
	getline(cin, email);

	cout << "\nPassword: ";
	getline(cin, password);

	cout << endl;

	if(data->signIn(email, password))
		signedInMenu(data);
	else{
		cout << "Wrong email and password combination.\n";
		mainMenu(data);
	}
}

void signUp(Data* data){
	clearScreen();
	string name, email, passwordOne = " ", passwordTwo = "", phoneNumber, location;

	unsigned int i = 0;
	do{
		if(i > 0)
			cout << "\nPlease introduce a valid email.";
		cout << "\nWhat is your email?\n";
		getline(cin, email);
		i++;
	}while(email.find("@") == -1 || email.find_last_of(".") == -1 || email.find("@") > email.find_last_of("."));

	i = 0;
	do{
		if(i > 0)
			cout << "\nYour passwords do not match.";
		cout << "\nWhat is your password?\n";
		getline(cin, passwordOne);
		cout << "\nPlease, repeat your password.\n";
		getline(cin, passwordTwo);
		i++;
	}while(passwordOne != passwordTwo);

	cout << "\nWhat is your name?\n";
	getline(cin, name);

	cout << "\nWhat is your phone number?\n";
	getline(cin, phoneNumber);

	cout << "\nWhere are you from? (City, county, district)\n";
	getline(cin, location);
	location = "Maia, Maia, Porto"; //temporary until there is a checking function
	Location loc(location);

	User u1(email, passwordOne, name, phoneNumber, loc);

	//showEmail
	string answer;
	cout << "\nWould you like to show your email in your advertisements? (Y/N)\n";
	i = 0;
	do{
		if(i > 0)
			cout << "Please introduce a valid option. (Y/N)\n";
		getline(cin,answer);
		if(answer == "N" || answer == "n")
			u1.setShowEmail(false);
		i++;
	}while(answer != "Y" && answer != "y" && answer != "N" && answer != "n");

	//showName
	cout << "\nWould you like show your name in your advertisements? (Y/N)\n";
	i = 0;
	do{
		if(i > 0)
			cout << "Please introduce a valid option. (Y/N)\n";
		getline(cin,answer);
		if(answer == "N" || answer == "n")
			u1.setShowName(false);
		i++;
	}while(answer != "Y" && answer != "y" && answer != "N" && answer != "n");


	//showPhoneNumber
	cout << "\nWould you like to show your phone number in your advertisements? (Y/N)\n";
	i = 0;
	do{
		if(i > 0)
			cout << "Please introduce a valid option. (Y/N)\n";
		getline(cin,answer);
		if(answer == "N" || answer == "n")
			u1.setShowPhoneNumber(false);
		i++;
	}while(answer != "Y" && answer != "y" && answer != "N" && answer != "n");

	data->signUp(u1);
	cout << loc;
	cout << "\nYour profile has been created. You may now sign in.\n";
	mainMenu(data);
}

void exitApp(Data* data){
	if(!data->saveUsers())
		exit(1);
	exit(0);
}

void signedInMenu(Data* data){
	clearScreen();
	OptionMenu menu(data);
	menu.addOption("Search", &search);
	menu.addOption("Create buying advertisement", &createBuyingAd);
	menu.addOption("Create selling advertisement", &createSellingAd);
	menu.addOption("Edit advertisement", &editAd);
	menu.addOption("Delete advertisement", &removeAd);
	menu.addOption("Sign out", &signOut);
	menu.addOption("Exit", &exitApp);
	menu.createMenu();
}

void createSellingAd(Data* data){
	clearScreen();
	string title, description = "", tmp = "", category, condition;
	float price;

	cout << "Title: ";
	getline(cin, title);

	cout << "\nCategory: ";
	do{
		getline(cin, category);
	}while(/*!isValidCategory(category)*/false); //TODO check if category is valid
	Category cat = Others;

	cout << "\nDescription: ";
	getline(cin, description);

	cin.clear();

	cout << "\nPrice: ";
	cin >> price;
	cin.ignore();
	cin.clear();

	cout << "\nProduct condition: ";
	do{
		getline(cin, condition);
	}while(/*!isValidCategory(category)*/false); //TODO check if category is valid
	Condition cond = New;

	Advertisement* ad = new Sale(data->getSignedInUser(), title, cat, description, cond, price);

	cout << "\nIs the price negotiable? (Y/N)\n";
	string answer;
	unsigned int i = 0;
	do{
		if(i > 0)
			cout << "Please introduce a valid option. (Y/N)\n";
		getline(cin,answer);
		if(answer == "N" || answer == "n")
			ad->setNegotiable(false);
		i++;
	}while(answer != "Y" && answer != "y" && answer != "N" && answer != "n");

	data->addAdvertisement(ad);
	cout << "Ad has been successfully created";
	signedInMenu(data);
}

void createBuyingAd(Data* data){
	clearScreen();
	string title, description = "", tmp = "", category, condition;
	float price;

	cout << "\nTitle: ";
	getline(cin, title);

	cout << "\nCategory: ";
	do{
		getline(cin, category);
	}while(/*!isValidCategory(category)*/false); //TODO check if category is valid
	Category cat = Others;

	cout << "\nDescription: ";
	getline(cin, description);

	cout << "\nPrice: ";
	cin >> price;
	cin.ignore();
	cin.clear();

	Advertisement* ad = new Purchase(data->getSignedInUser(), title, cat, description, price);

	cout << "\nIs the price negotiable? (Y/N)\n";
	string answer;
	unsigned int i = 0;
	do{
		if(i > 0)
			cout << "Please introduce a valid option. (Y/N)\n";
		getline(cin,answer);
		if(answer == "N" || answer == "n")
			ad->setNegotiable(false);
		i++;
	}while(answer != "Y" && answer != "y" && answer != "N" && answer != "n");


	data->addAdvertisement(ad);
	cout << "Ad has been successfully created\n";
	signedInMenu(data);
}

void editAd(Data* data){
	clearScreen();
	//TODO implement function
	signedInMenu(data);
}

void removeAd(Data* data){
	clearScreen();
	//temporary. will create a menu afterwards.
	string title;
	cout << "Insert the title of the advertisement you want to delete.\n";
	//data->removeAdvertisement(title);
	cout << "WIP. Ad has been successfully removed.\n";
	signedInMenu(data);
}

void signOut(Data* data){
	data->signOut();
	mainMenu(data);
}

void interested(User* user){
	clearScreen();
	string contact, message;
	cout << "\nPlease enter your contact: ";
	getline(cin, contact);
	cout << "\nPlease enter a message to the announcer.\n";
	getline(cin, message);
	//sendEmail(user->getEmail(), contact, message);
	cout << "Message sent.\n";
}
