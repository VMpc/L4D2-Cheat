/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#include "commands.h"
#include "game.h"
#include "utils.h"

char *toggleEsp(Game *game, char **args)
{
    char NOPs[7] = {(char)0x90, (char)0x90, (char)0x90,
			(char)0x90, (char)0x90, (char)0x90, (char)0xFF};

	char Revert[8] = {(char)0x0F, (char)0x83, (char)0xD1,
			(char)0x01, (char)0x00, (char)0x00, (char)0xFF, (char)0x90};

    if (strstr(args[1], "on"))
    {
        pokeAddr(game->pid,
			game->clientModule + Offsets.GlowAddr, NOPs, 7);
        return "Turned on esp";
    } else if (strstr(args[1], "off"))
    {
		pokeAddr(game->pid,
			game->clientModule + Offsets.GlowAddr, Revert, 8);
		return "Turned off esp";
	}

    return "What were you trying to do?";
}

char *toggleNoBoom(Game *game, char **args)
{
    char NOPs[8] = {(char)0x90, (char)0x90, (char)0x90,
			(char)0x90, (char)0x90, (char)0x89, (char)0x1C, (char)0x24};

	char Revert[8] = {(char)0xE8, (char)0x46, (char)0x2B,
			(char)0xFF, (char)0xFF, (char)0x89, (char)0x1C, (char)0x24};

    if (strstr(args[1], "on"))
    {
        pokeAddr(game->pid,
			game->clientModule + Offsets.BoomAddr, NOPs, 8);
        return "Turned on no-boom";
    } else if (strstr(args[1], "off")) {
		pokeAddr(game->pid,
			game->clientModule + Offsets.BoomAddr, Revert, 8);
		return "Turned off no-boom";
	}

    return "What were you trying to do?";
}
