#include "Piece.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>


Piece::Piece(const std::string& sides, char delim, int id) : 
	m_id{ id }, m_border{ false }, m_corner{ false }
{
	auto ss = std::stringstream{ sides };
	auto item = std::string{};
	auto ceroCount = 0, separationCounter = 0, separation = 0, nonCeroCount = 0;
	for (auto i = 0u; i < m_sides.size() && std::getline(ss, item, delim); i++)
	{
		if (item == "0")
		{
			if (ceroCount == 0)
				separationCounter = 0;
			else
				separation = separationCounter;
			ceroCount++;
		}
		else
		{
			separationCounter++;
		}
		m_sides[i] = std::move(item);
	}

	classify(ceroCount, separation);
}

Piece::~Piece()
{
}

void Piece::rotate()
{
	std::rotate(m_sides.begin(), m_sides.begin() + 1, m_sides.end());
}

bool Piece::isCorner() const noexcept
{
	return m_corner;
}

bool Piece::isBorder() const noexcept
{
	return m_border;
}

std::string_view Piece::side(size_t index) const
{
	return m_sides.at(index);
}

bool Piece::tryFitting(const std::array<std::string_view, m_numsides>& hole) 
{
	if (numSides() != hole.size())
	{
		return false;
	}

	for (auto i = 0u, tries = 0u; i < m_sides.size();)
	{
		if (m_sides[i] != hole[i] && hole[i] != "-") 
		{
			if (tries < m_sides.size())
			{
				i = 0u;
				tries++;
				rotate();
			}
			else
			{
				return false;
			}
		}
		else
		{
			i++;
		}
	}

	return true;
}

int Piece::getID() const noexcept
{
	return m_id;
}

void Piece::classify(int ceroCount, int separation) noexcept
{
	switch (ceroCount) {
	case 0:
		m_corner = false;
		m_border = false;
		break;
	case 1:
		m_corner = false;
		m_border = true;
		break;
	case 2:
		if (separation == 1) 
		{
			m_corner = false;
			m_border = true;
		}
		else 
		{
			m_corner = true;
			m_border = false;
		}
		break;
	case 3:
		m_corner = true;
		m_border = false;
		break;
	};
}
