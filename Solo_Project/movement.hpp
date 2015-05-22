#pragma once

#include "SFML\Graphics.hpp"


#include <functional>
#include <iostream>


#include "entity.hpp"

/*
    Movement class allows any non-playable entities (enemy, bullet, etc.)
	to have a pre-defined movement based on the following parameters:

	'*' denotes optional since some movements are very simple, like linear

	  String: "name" - search the name on the collection of movement patterns.
	          if the name matches the pattern, a specified function will be called
			  to give the entity movement
	* Vector2f: vertex ([0],[1]) - (x,y) or (y,x).
	          The first index of vector2f will be used
			  as primary movement axis (x or y).
			  The second index will be reserved for
			  more complicated movements (y or x).
	  float []: initializer parameters - depending on formula, more variables
	          will be needed inside the array.
			  Along with the name being searched, the array must have the correct amount
			  of elements to get a specific function
*/


/*
   This file will also make use of function pointers,
   a helpful, but rarely used technique in C++.
   
   Function Pointers are like pointers, but have some special rules
   to use them correctly. One special rule is that pointers to functions
   CANNOT be dereferenced, if they are used as an argument.
   
   To declare a function pointer, it must have the following:

   std::function<returnType (Arguments) *funcPtr>

   For the function pointer to work, there must be functions
   that match the same return type and its arguments

   example:

   std::function<int (int, int)> *mathOp; // required return and argument types

   // matching return and argument types
   int add(int a, int b) {return a + b;};
   int sub(int a, int b) {return a - b;};

   main (){

   std::function <int (int, int)> *plus = &add;
   std::function <int (int, int)> *minus = &sub;

   std::cout << "add function: " << (*plus) (2,3) << endl;
   std::cout << "add function: " << (*minus) (2,3) << endl;


   }

*/

class Movement {

private:

	// constructor parameters
	std::vector<float> args;
	std::string name;
	
	sf::Vector2f vertex;

	std::vector <sf::Vector2f> waypoints;

	// other variables
	int waypoint_idx;

	sf::Vector2f curr_waypoint, next_waypoint;
	float move_angle;

public:

	// Constructor

	// When initalizing values with an initializer list,
	// use {} for single items and () for collection of items


	// For movements without waypoints

	Movement(std::string name, sf::Vector2f vertex, std::vector<float>args) :
		name(name), vertex(vertex), args(args)
	{};

	// For movements with waypoints

	Movement(sf::Vector2f vertex, std::vector<sf::Vector2f> _waypoints):
		vertex(vertex)
	{
		/*
		  We shall initalize the waypoints based on offset from the vertex
		  (i.e. _waypoints[0] = (10, 10), then this->waypoints[0] = vertex + _waypoints[0])

		*/
		for (auto i = _waypoints.begin(); i != _waypoints.end(); ++i){
			// Add the offset from the waypoint parameter to the vertex
			// so that can be added to the object's waypoint list
			sf::Vector2f itor_vec = sf::Vector2f(
				(*i).x + vertex.x,
				(*i).y + vertex.y);
			this->waypoints.push_back(itor_vec);
		}
		curr_waypoint = vertex;
		next_waypoint = waypoints[0];
		waypoint_idx = 0;
		setMoveAngle();
	};

	// copy constructor
	Movement(const Movement&);

	// overloaded assignment operator
	Movement& operator= (const Movement&);

	// destructor
	~Movement();


	// functions

	void update(Entity&);

	void setName(std::string name) { this->name = name; };

	void setEntityVertex(sf::Vector2f vertex) { this->vertex = vertex; };
	
	void setFloatArgs(std::vector<float> args) { this->args = args; };

	// The intialization behaves like the constructor for waypoints
	void setWaypoints(std::vector<sf::Vector2f> _waypoints){
		this->waypoints.clear();
		for (auto i = _waypoints.begin(); i != _waypoints.end(); ++i){
			sf::Vector2f itor_vec = sf::Vector2f(
				(*i).x + vertex.x,
				(*i).y + vertex.y);
			this->waypoints.push_back(itor_vec);
		}
		curr_waypoint = vertex;
		next_waypoint = waypoints[0];
		waypoint_idx = 0;
		setMoveAngle();
	};
	std::vector <sf::Vector2f> getWaypoints(){ return waypoints; };
	sf::Vector2f getVertex() { return vertex; };


	void lookupMovement(Entity&, std::string);
	void lookupMovement(Entity&, std::vector<sf::Vector2f>);

	void setMoveAngle();

	// Functions used to define specific movements for entities
	// Will be used for function lookup table

	/* The required parameters are in order:
	   1 - Entity&: need the entity to move it
	   2 - sf::Vector2f: vertex will be used for some complex equations
	   3 - float*: number of float values for equations
	*/
	void circle(Entity&, sf::Vector2f, std::vector<float>);
	void straight(Entity&, sf::Vector2f, std::vector<float>);
	void sinusodial(Entity&, sf::Vector2f, std::vector<float>);
	void walk(Entity&, sf::Vector2f, std::vector<float>);



};

// Why do I need to have templates if I can do a deep copy constructor?

class MovementTemplate{
private:
	sf::Vector2f vertex;
	std::string name;
	std::vector <float> args;
	std::vector<sf::Vector2f> waypoints;

public:

	MovementTemplate(std::string name, sf::Vector2f vertex, std::vector<float> args):
		name(name), vertex(vertex), args(args)
	{};

	MovementTemplate(sf::Vector2f vertex, std::vector<sf::Vector2f> waypoints):
		vertex(vertex), waypoints(waypoints)
	{};

	sf::Vector2f getVertex() { return vertex; };
	std::string getName(){ return name; };
	std::vector <float> getArgs() { return args; };
	std::vector<sf::Vector2f> getWaypoints() { return waypoints; };
};