#include "movement.hpp"
#include <cmath>

#include "enemyTerrain.hpp"
#include "terrain.hpp"

#include "player.hpp" // used for chase functions


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
		{ "bounds", &Movement::bounds },
		{ "bounds_chase", &Movement::bounds_chase },
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

	// update waypoint movement with cycle option

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

void Movement::setMoveAngle(float &_move_angle){
	sf::Vector2f point_difference;
	point_difference.x = next_waypoint.x - curr_waypoint.x;
	point_difference.y = next_waypoint.y - curr_waypoint.y;

	_move_angle = atan2f(point_difference.y, point_difference.x);
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

	this->setEntityVertex(sf::Vector2f(vertex.x - (entity.getSpeed() / 2 ), vertex.y));

	entity.setPosition(
		radius * cos((entity.getTicks() * entity.getSpeed()) * deg_to_rad) + getVertex().x,
		radius * sin((entity.getTicks() * entity.getSpeed()) * deg_to_rad) + getVertex().y);

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
		amplitude * sin( (2 * pi) * period * entity.getTicks()) + getVertex().y);

}


/*
  There will be 3 bound types depending on float value
  0 horizontal
  1 vertical
  2 both
*/
void Movement::bounds(Entity& entity, sf::Vector2f vertex, std::vector<float> params){
	if (params.size() != 2){
		std::cerr << "Need two float values for sine: " <<
			"angle(0) and bound_type[0-2](1)" << std::endl;
		return;
	}

	float angle_x;
	float angle_y;

	float pi = 3.14;
	float deg_to_rad = pi / 180;

	// Now first calculate the current components (X and Y)

	angle_x = cos(params[0] * deg_to_rad);
	angle_y = sin(params[0] * deg_to_rad);

	// keep moving entity from going out of bounds
	sf::Vector2f tl_window_view = entity.getScene()->game->window.mapPixelToCoords(
		sf::Vector2i(0, 0));
	sf::Vector2f br_window_view = entity.getScene()->game->window.mapPixelToCoords(
		sf::Vector2i(entity.getScene()->game->window.getSize().x,
		entity.getScene()->game->window.getSize().y));

	// then change values of the compnents through bound check
	if (params[1] == 0 || params[1] == 2){
		if (entity.getGlobalBounds().left < tl_window_view.x){
			entity.setPosition(tl_window_view.x + entity.getGlobalBounds().width / 2,
				entity.getPosition().y);
			angle_x = angle_x > 0 ? angle_x * -1 : angle_x;
		}
		if (entity.getPosition().x >= br_window_view.x
			- entity.getGlobalBounds().width / 2){
			entity.setPosition(br_window_view.x - entity.getGlobalBounds().width / 2,
				entity.getPosition().y);
			angle_x = angle_x < 0 ? angle_x * -1 : angle_x;
		}
	}
	if (params[1] == 1 || params[1] == 2){
		if (entity.getGlobalBounds().top < tl_window_view.y){
			entity.setPosition(entity.getPosition().x,
				tl_window_view.y + entity.getGlobalBounds().height / 2);
			angle_y = angle_y < 0 ? angle_y * -1 : angle_y;
		}
		if (entity.getPosition().y >= br_window_view.y
			- entity.getGlobalBounds().height / 2){
			entity.setPosition(entity.getPosition().x,
				br_window_view.y - entity.getGlobalBounds().height / 2);
			angle_y = angle_y > 0 ? angle_y * -1 : angle_y;
		}
	}

	// then update the angle with changed compounds
	// and convert back to degrees

	float deg_angle = atan2f(angle_y, angle_x) * (1 / deg_to_rad);

	params[0] = deg_angle;
	args[0] = params[0];


	entity.move(entity.getSpeed() * cos(params[0] * deg_to_rad),
		entity.getSpeed() * sin(params[0] * deg_to_rad));


}


void Movement::bounds_chase(Entity& entity, sf::Vector2f vertex, std::vector<float> params){
	if (params.size() != 3){
		std::cerr << "Need three float values for sine: " <<
			"angle(0), bound_type[0-2](1), and chase delay(2)" << std::endl;
		return;
	}
	chase_time_set = params[2];
	set_chase_mode(entity);
	if (is_chasing){
		chase_mode(entity);
		return;
	}


	float angle_x;
	float angle_y;

	float pi = 3.14;
	float deg_to_rad = pi / 180;

	// Now first calculate the current components (X and Y)

	angle_x = cos(params[0] * deg_to_rad);
	angle_y = sin(params[0] * deg_to_rad);

	// keep moving entity from going out of bounds
	sf::Vector2f tl_window_view = entity.getScene()->game->window.mapPixelToCoords(
		sf::Vector2i(0, 0));
	sf::Vector2f br_window_view = entity.getScene()->game->window.mapPixelToCoords(
		sf::Vector2i(entity.getScene()->game->window.getSize().x,
		entity.getScene()->game->window.getSize().y));

	// then change values of the compnents through bound check
	if (params[1] == 0 || params[1] == 2){
		if (entity.getGlobalBounds().left < tl_window_view.x){
			entity.setPosition(tl_window_view.x + entity.getGlobalBounds().width / 2,
				entity.getPosition().y);
			angle_x = angle_x > 0 ? angle_x * -1 : angle_x;
		}
		if (entity.getPosition().x >= br_window_view.x
			- entity.getGlobalBounds().width / 2){
			entity.setPosition(br_window_view.x - entity.getGlobalBounds().width / 2,
				entity.getPosition().y);
			angle_x = angle_x < 0 ? angle_x * -1 : angle_x;
		}
	}
	if (params[1] == 1 || params[1] == 2){
		if (entity.getGlobalBounds().top < tl_window_view.y){
			entity.setPosition(entity.getPosition().x,
				tl_window_view.y + entity.getGlobalBounds().height / 2);
			angle_y = angle_y < 0 ? angle_y * -1 : angle_y;
		}
		if (entity.getPosition().y >= br_window_view.y
			- entity.getGlobalBounds().height / 2){
			entity.setPosition(entity.getPosition().x,
				br_window_view.y - entity.getGlobalBounds().height / 2);
			angle_y = angle_y > 0 ? angle_y * -1 : angle_y;
		}
	}

	// then update the angle with changed compounds
	// and convert back to degrees

	float deg_angle = atan2f(angle_y, angle_x) * (1 / deg_to_rad);

	params[0] = deg_angle;
	args[0] = params[0];


	entity.move(entity.getSpeed() * cos(params[0] * deg_to_rad),
		entity.getSpeed() * sin(params[0] * deg_to_rad));




}

void Movement::chase_mode(Entity& entity){

	if (entity.getGlobalBounds().contains(next_waypoint)){
		curr_waypoint = next_waypoint;
		next_waypoint = 
			 ++waypoint_idx < chase_waypoints.size() ? chase_waypoints[waypoint_idx]: sf::Vector2f(0,0);
		setMoveAngle(chase_angle);
	}

	if (waypoint_idx >= chase_waypoints.size()){
		is_chasing = false;
		chase_time_delay = 0;
		chase_waypoints.clear();
		return;
	}

	entity.move(entity.getSpeed() * cos(chase_angle),
		entity.getSpeed() * sin(chase_angle));
}

void Movement::set_chase_mode(Entity& entity){
	chase_time_delay++;
	if (!is_chasing && chase_time_delay % chase_time_set == 0){
		is_chasing = true;

		// get the window position for the last waypoint:

		sf::Vector2f window_bounds = entity.getScene()->game->window.mapPixelToCoords(
			sf::Vector2i(entity.getScene()->game->window.getSize()));
		sf::Vector2f window_corners = entity.getScene()->game->window.mapPixelToCoords(
			sf::Vector2i(0,0));

		// Set up the waypoints and angle so enemy can target the player and back
		chase_waypoints.push_back(sf::Vector2f(window_corners.x + entity.getGlobalBounds().width,
			                      entity.getPosition().y));
		chase_waypoints.push_back(sf::Vector2f(window_bounds.x + entity.getGlobalBounds().width,
			                      entity.getPosition().y));

		curr_waypoint = sf::Vector2f(entity.getPosition().x, entity.getPosition().y);
		next_waypoint = chase_waypoints[0];
		waypoint_idx = 0;

		setMoveAngle(chase_angle);
	}
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
