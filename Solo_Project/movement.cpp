#include "movement.hpp"
#include <cmath>

#include "enemyTerrain.hpp"
#include "terrain.hpp"


// constructors

Movement::Movement(const Movement& source){
	
	// All of the arguments are copyable at shallow level
	name = source.name;
	waypoints = source.waypoints;
	args = source.args;
	vertex = source.vertex;

	waypoint_idx = source.waypoint_idx;

	curr_waypoint = source.curr_waypoint;
	next_waypoint = source.next_waypoint;
	move_angle = source.move_angle;

}

// destructor
Movement::~Movement() {

}

// overloaded assignment operator
Movement& Movement::operator=(const Movement& source){
	// check for self assignment
	if ( this == &source) return *this;

	// All of the arguments are copyable at shallow level
	///*
	name = source.name;
	waypoints = source.waypoints;
	args = source.args;
	vertex = source.vertex;

	waypoint_idx = source.waypoint_idx;

	curr_waypoint = source.curr_waypoint;
	next_waypoint = source.next_waypoint;
	move_angle = source.move_angle;

	//*/



	return *this;

}



void Movement::update(Entity& entity){

	/* 
	if movement has a string name and float args, then
	look up movement by name

	otherwise if movement has a vector of sf::Vector2f waypoints
	then look up movement by waypoints
	*/
	if (!name.empty() && !args.empty()){
		lookupMovement(entity, this->name);
		return;
	}

	if (!waypoints.empty()){
		lookupMovement(entity, this->waypoints);
		return;
	}

	// return error print and close program if none of these options are valid


}

// First look up the table of function pointers to find its name
// if the name is not found after the search, stop program
// and tell the user "movement name not found"

void Movement::lookupMovement(Entity& entity, std::string name){

	static std::map < std::string, void (Movement::*)
		(Entity&, sf::Vector2f, std::vector<float>) >  table
	{
		{"straight", &Movement::straight},
		{"circle", &Movement::circle},
		{ "sine", &Movement::sinusodial },
		{ "walk", &Movement::walk },
		{ "stay", &Movement::stay }
	};

	auto entry = table.find(name);
	if (entry != table.end()){
		(this->*(entry->second))(entity, vertex, args);
	}
	else {
		std::cerr << "Cannot find movement with the name " << name << std::endl;
		std::exit(1);
	}

}

void Movement::lookupMovement(Entity& entity, std::vector<sf::Vector2f> waypoints){
	/*
       Check if the entity has reached to the next point.
	   If true, then do the following:
	     Check if the index has not reached the end
		   if true, move current waypoint to next one and move next waypoint by index of 1
		   recalculate the movement angle with the new waypoint values
		if false, then assign current waypoint to next and keep moving

	  Now move the entity from one waypoint to another.
	*/

	if (entity.getGlobalBounds().contains(next_waypoint) &&
		waypoint_idx < waypoints.size() - 1){
		curr_waypoint = next_waypoint;
		next_waypoint = waypoints[++waypoint_idx];
		setMoveAngle();
	}

	entity.move(entity.getSpeed() * cos(move_angle),
		entity.getSpeed() * sin(move_angle));


}

void Movement::setMoveAngle(){
	sf::Vector2f point_difference;
	point_difference.x = next_waypoint.x - curr_waypoint.x;
	point_difference.y = next_waypoint.y - curr_waypoint.y;

	move_angle = atan2f (point_difference.y, point_difference.x);
}


// for each individual function involving movement,
// Check that the correct number of float arguments are there.
// if the number of arguments are incorrect,
// then stop the program and tell the user
// the correct array size and its arguments in order.

void Movement::circle(Entity& entity, sf::Vector2f vertex, std::vector<float> params){
	if (params.size() != 1){
		std::cerr << "Need only one float value for circle: radius" << std::endl;
		return;
	}
	float pi = 3.14;
	float deg_to_rad = pi / 180;

	float radius = params[0];

	entity.setPosition(
		radius * cos((entity.getTicks() * entity.getSpeed()) * deg_to_rad) + vertex.x,
		radius * sin((entity.getTicks() * entity.getSpeed()) * deg_to_rad) + vertex.y);

	// check with breakpoint


}
void Movement::straight(Entity& entity, sf::Vector2f vertex, std::vector<float> params){
	if (params.size() != 1){
		std::cerr << "Need only one float value for stright: angle" << std::endl;
		return;
	}

	float pi = 3.14;
	float deg_to_rad = pi / 180;


	entity.move(entity.getSpeed() * cos(params[0] * deg_to_rad),
		        entity.getSpeed() * sin(params[0] * deg_to_rad));


}

void Movement::sinusodial(Entity& entity, sf::Vector2f vertex, std::vector<float> params){

	if (params.size() != 2){
		std::cerr << "Need two float values for sine: " <<
			"amplitude(0) and period(1)" << std::endl;
		return;
	}

	float pi = 3.14;

	/* since it is a horizontal shooting game,
	   we will move forward with x
	   and use sine movement on y
	*/

	float amplitude = params[0];
	float period = params[1];
	entity.setPosition( // check with breakpoint



		entity.getPosition().x - entity.getSpeed(),
		amplitude * sin( (2 * pi) * period * entity.getTicks()) + vertex.y);

} 


// This behavior is exclusive for terrain enemy
void Movement::walk(Entity& entity, sf::Vector2f vertex, std::vector<float> params){
	if (EnemyTerrain* enemyLand = dynamic_cast<EnemyTerrain*> (&entity)){
		// implement walking behavior

		enemyLand->move(enemyLand->getSpeed(), enemyLand->getFallSpeed());
		
		// will have to develop more
		// if the enemy has a turn-around behavior

	}
}

// This behavior is exclusive for terrain enemy
void Movement::stay(Entity& entity, sf::Vector2f vertex, std::vector<float> params){
	if (EnemyTerrain* enemyLand = dynamic_cast<EnemyTerrain*> (&entity)){
		// implement stay behavior
		enemyLand->move(0,enemyLand->getFallSpeed());

	}
}
