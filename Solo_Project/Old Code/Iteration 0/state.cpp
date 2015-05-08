#include "state.hpp"

State::State(Game* p, const std::string& name) :
rGame{ p },
rWindow{ rGame->gameWindow },
//rName{ name },
rIsActive{ true }
{

}

void State::pushRequest(std::string s){
	rIsActive = false;
}

void State::popRequest(){

}

void State::swapRequest(std::string s){

}

void State::returnRequest(std::string s){

}

void State::clearRequest(){

}
