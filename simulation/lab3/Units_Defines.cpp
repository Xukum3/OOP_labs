#include "Units.h"

std::unordered_map<int, double> Soldier::attractive = {
	{soldier, 2 },
	{defender, 3 },
	{invader, 2 },
	{spawner, 0 },
	{acid, 0 },
	{meal, 0 },
	{salt, 0 } 
};

std::unordered_map<int, double> Soldier::own_attractive = {
	{ soldier, 0 },
	{ defender, 0 },
	{ invader, 0 },
	{ spawner, 0 },
	{ acid, 0 },
	{ meal, 0 },
	{ salt, 0 }
};



std::unordered_map<int, double> Soldier::protect_points_attractive = {
	{ soldier, 3 },
	{ defender, 3 },
	{ invader, 3 },
	{ spawner, 0 },
	{ acid, 0 },
	{ meal, 0 },
	{ salt, 0 } 
};

std::unordered_map<int, double> Soldier::protect_points_own_attractive = {
	{ soldier, 0 },
	{ defender, 0 },
	{ invader, 0 },
	{ spawner, 4 },
	{ acid, 4 },
	{ meal, 4 },
	{ salt, 4 }
};



std::unordered_map<int, double> Soldier::user_attractive = {
	{ soldier, 0 },
	{ defender, 0 },
	{ invader, 0 },
	{ spawner, 0 },
	{ acid, 0 },
	{ meal, 0 },
	{ salt, 0 } 
};


std::unordered_map<int, double> Soldier::user_own_attractive = {
   { soldier, 0 },
   { defender, 0 },
   { invader, 0 },
   { spawner, 0 },
   { acid, 0 },
   { meal, 0 },
   { salt, 0 } 
};

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

std::unordered_map<int, double> Defender::attractive = {
	{soldier, -1 },
	{defender, -1 },
	{invader, -1 },
	{spawner, 0 },
	{acid, 3 },
	{meal, 3 },
	{salt, 3 }
};

std::unordered_map<int, double> Defender::own_attractive = {
	{ soldier, 0 },
	{ defender, 0 },
	{ invader, 0 },
	{ spawner, 0 },
	{ acid, 1 },
	{ meal, 1 },
	{ salt, 1 }
};



std::unordered_map<int, double> Defender::protect_points_attractive = {
	{ soldier, 1 },
	{ defender, 1 },
	{ invader, 1 },
	{ spawner, 0 },
	{ acid, 0 },
	{ meal, 0 },
	{ salt, 0 }
};

std::unordered_map<int, double> Defender::protect_points_own_attractive = {
	{ soldier, 0 },
	{ defender, 0 },
	{ invader, 0 },
	{ spawner, 4 },
	{ acid, 4 },
	{ meal, 4 },
	{ salt, 4 }
};



std::unordered_map<int, double> Defender::user_attractive = {
	{ soldier, 0 },
	{ defender, 0 },
	{ invader, 0 },
	{ spawner, 0 },
	{ acid, 0 },
	{ meal, 0 },
	{ salt, 0 }
};

std::unordered_map<int, double> Defender::user_own_attractive = {
   { soldier, 0 },
   { defender, 0 },
   { invader, 0 },
   { spawner, 0 },
   { acid, 0 },
   { meal, 0 },
   { salt, 0 }
};
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

std::unordered_map<int, double> Invader::attractive = {
	{soldier, 2 },
	{defender, 2 },
	{invader, 2 },
	{spawner, 4 },
	{acid, 2 },
	{meal, 2 },
	{salt, 2 }
};

std::unordered_map<int, double> Invader::own_attractive = {
	{ soldier, 0 },
	{ defender, 0 },
	{ invader, 0 },
	{ spawner, 0 },
	{ acid, 0 },
	{ meal, 0 },
	{ salt, 0 }
};



std::unordered_map<int, double> Invader::protect_points_attractive = {
	{ soldier, 2 },
	{ defender, 2 },
	{ invader, 2 },
	{ spawner, 0 },
	{ acid, 0 },
	{ meal, 0 },
	{ salt, 0 }
};

std::unordered_map<int, double> Invader::protect_points_own_attractive = {
	{ soldier, 0 },
	{ defender, 0 },
	{ invader, 0 },
	{ spawner, 4 },
	{ acid, 4 },
	{ meal, 4 },
	{ salt, 4 }
};



std::unordered_map<int, double> Invader::user_attractive = {
	{ soldier, 0 },
	{ defender, 0 },
	{ invader, 0 },
	{ spawner, 0 },
	{ acid, 0 },
	{ meal, 0 },
	{ salt, 0 }
};

std::unordered_map<int, double> Invader::user_own_attractive = {
   { soldier, 0 },
   { defender, 0 },
   { invader, 0 },
   { spawner, 0 },
   { acid, 0 },
   { meal, 0 },
   { salt, 0 }
};