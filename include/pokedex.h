#pragma once

#include <3ds/types.h>

/**
 * @brief Retrieves the National Pokdédex based on the species' index.
 * @param index The index of the species.
 * @return The National Pokédex number.
 */
uint8_t pokedexGetNational(uint8_t index);

/**
 * @brief Adds an entry to the owned Pokédex.
 * @param species The species of the Pokémon.
 */
void pokedexAddOwned(uint8_t species);

/**
 * @brief Adds an entry to the seen Pokédex.
 * @param species The species of the Pokémon.
 */
void pokedexAddSeen(uint8_t species);
