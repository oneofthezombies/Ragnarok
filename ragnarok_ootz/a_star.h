#pragma once

#include <algorithm>
#include <iterator>
#include <vector>
#include <deque>
#include <tuple>
#include <map>

#include "vector2.h"

struct AStar
{
    AStar()
        : _direction_factors(std::vector<std::pair<int, int>>{})
        , _visits(std::vector<std::vector<bool>>{})
        , _costs(std::vector<std::vector<std::tuple<int, int, int>>>{})
        , _start(std::pair<int, int>{})
        , _destination(std::pair<int, int>{})
    {
    }

    std::deque<std::pair<int, int>> operator()(std::vector<std::vector<int>> const& map, 
                                   std::pair<int, int> const& start, 
                                   std::pair<int, int> const& destination)
    {
        if (_is_out_of_range(map, destination) ||
            _is_no_go_area(map, destination) ||
            _is_same(start, destination))
            return std::deque<std::pair<int, int>>{};

        _visits.resize(map.size(), std::vector<bool>(map.front().size(), false));
        _costs.resize(map.size(), std::vector<std::tuple<int, int, int>>(map.front().size(), std::tuple<int, int, int>{}));

        _start = start;
        _destination = destination;

        _make_priority_direction_factors(start, destination);

        return _search(map);
        //return _search2(map, start);
    }

    std::vector<std::vector<std::tuple<int, int, int>>> getCosts() const
    {
        return _costs;
    }

private:
    std::deque<std::pair<int, int>> _search2(std::vector<std::vector<int>> const& map, std::pair<int,int> const& position)
    {
        if (!_is_visitable(map, position))
            return std::deque<std::pair<int, int>>{};

        if (_is_same(position, _destination))
            return std::deque<std::pair<int, int>>{ position };

        _visit(position);

        std::deque<std::pair<int, int>> available_positions;
        for (auto factor : _direction_factors)
        {
            auto direction = _add(position, factor);
            if (_is_visitable(map, direction))
            {
                if (!_is_evaluated(direction))
                    _evaluate(direction);

                available_positions.emplace_back(direction);
            }
        }

        if (available_positions.empty())
            return std::deque<std::pair<int, int>>{};

        if (available_positions.size() > 1)
            sort(available_positions.begin(), available_positions.end(),
                 [this](std::pair<int, int> const& lhs, std::pair<int, int> const& rhs)
            {
                return std::get<2>(_costs[lhs.second][lhs.first]) < std::get<2>(_costs[rhs.second][rhs.first]);
            });

        std::deque<std::pair<int, int>> nexts;
        do
        {
            nexts.emplace_back(available_positions.front());
            available_positions.pop_front();
        }
        while (!available_positions.empty() &&
               std::get<2>(_costs[nexts.back().second][nexts.back().first]) >= std::get<2>(_costs[available_positions.front().second][available_positions.front().first]));

        _make_priority_direction_factors(_destination, _start);

        std::deque<std::deque<std::pair<int, int>>> results;
        for (auto next : nexts)
            results.emplace_back(_search2(map, next));

        auto rowest = results.begin();
        for (auto it = next(results.begin()); it != results.end(); ++it)
            if (rowest->size() == 0 ||
                rowest->size() > it->size())
                rowest = it;
                
        if (rowest->empty())
            return std::deque<std::pair<int, int>>{};

        std::vector<std::pair<int, int>> visiteds;
        for (auto factor : _direction_factors)
        {
            auto direction = _add(position, factor);
            if (!_is_out_of_range(map, direction) &&
                !_is_no_go_area(map, direction) &&
                _is_visited(direction))
            {
                _visits[direction.second][direction.first] = false;

                if (_is_same(direction, _start))
                {
                    rowest->emplace_front(direction);
                    return *rowest;
                }

                visiteds.emplace_back(direction);
            }
        }

        if (visiteds.empty())
            return std::deque<std::pair<int, int>>{};

        if (visiteds.size() > 1)
            sort(visiteds.begin(), visiteds.end(), 
                 [this](std::pair<int,int> const& lhs, std::pair<int,int> const& rhs) 
            {
                return std::get<0>(_costs[lhs.second][lhs.first]) < std::get<0>(_costs[rhs.second][rhs.first]);
            });

        rowest->emplace_front(visiteds.front());

        return *rowest;
    }

    std::deque<std::pair<int, int>> _search(std::vector<std::vector<int>> const& map)
    {
        std::deque<std::pair<int, int>> ret;
        std::deque<std::pair<int, int>> queue;
        queue.emplace_back(_start);
        while (!queue.empty())
        {
            auto position = queue.front();
            queue.pop_front();

            if (_is_same(position, _destination))
            {
                ret.emplace_front(position);
                break;
            }

            _visit(position);

            std::deque<std::pair<int, int>> available_positions;
            for (auto factor : _direction_factors)
            {
                auto direction = _add(position, factor);
                if (_is_visitable(map, direction))
                {
                    if (!_is_evaluated(direction))
                        _evaluate(direction);

                    available_positions.emplace_back(direction);
                }
            }

            if (available_positions.empty())
                continue;

            sort(available_positions.begin(), available_positions.end(),
                 [this](std::pair<int, int> const& lhs, std::pair<int, int> const& rhs)
            {
                return std::get<2>(_costs[lhs.second][lhs.first]) < std::get<2>(_costs[rhs.second][rhs.first]);
            });

            std::pair<int, int> front;
            do
            {
                front = available_positions.front();
                available_positions.pop_front();
                queue.emplace_back(front);
            }
            while (!available_positions.empty() &&
                   std::get<2>(_costs[front.second][front.first]) >= std::get<2>(_costs[available_positions.front().second][available_positions.front().first]));

            //sort(available_positions.begin(), available_positions.end(),
            //     [this](std::pair<int, int> const& lhs, std::pair<int, int> const& rhs) 
            //{
            //    return std::get<1>(_costs[lhs.second][lhs.first]) < std::get<1>(_costs[rhs.second][rhs.first]);
            //});

            //if (!available_positions.empty())
            //    do
            //    {
            //        front = available_positions.front();
            //        available_positions.pop_front();
            //        queue.emplace_back(front);
            //    }
            //    while (!available_positions.empty() &&
            //           _costs[front.second][front.first] == _costs[available_positions.front().second][available_positions.front().first]);
        }

        if (ret.empty())
            return std::deque<std::pair<int, int>>{};

        _make_priority_direction_factors(_destination, _start);

        bool is_impossible = false;
        while (!is_impossible)
        {
            std::deque<std::pair<int, int>> visiteds;
            for (auto direciton_factor : _direction_factors)
            {
                auto direction = _add(ret.front(), direciton_factor);
                if (!_is_out_of_range(map, direction) && 
                    _is_visited(direction))
                {
                    if (_is_same(direction, _start))
                        return ret;

                    _visits[direction.second][direction.first] = false;
                    visiteds.emplace_back(direction);
                }
            }

            if (visiteds.empty())
                return std::deque<std::pair<int, int>>{};

            sort(visiteds.begin(), visiteds.end(), 
                 [this](std::pair<int, int> const& lhs, std::pair<int,int> const& rhs) 
            {
                return std::get<0>(_costs[lhs.second][lhs.first]) < std::get<0>(_costs[rhs.second][rhs.first]);
            });

            ret.emplace_front(visiteds.front());
        }

        return ret;
    }

    bool _is_out_of_range(std::vector<std::vector<int>> const& map, std::pair<int, int> const& position)
    {
        if (position.first < 0 ||
            static_cast<unsigned int>(position.first) >= map.front().size() ||
            position.second < 0 ||
            static_cast<unsigned int>(position.second) >= map.size())
            return true;

        return false;
    }

    bool _is_no_go_area(std::vector<std::vector<int>> const& map, std::pair<int, int> const& position)
    {
        if (map[position.second][position.first])
            return true;

        return false;
    }

    bool _is_visited (std::pair<int, int> const& position)
    {
        if (_visits[position.second][position.first])
            return true;

        return false;
    }

    void _visit(std::pair<int, int> const& position)
    {
        _visits[position.second][position.first] = true;
    }

    bool _is_same(std::pair<int, int> const& lhs, std::pair<int, int> const& rhs)
    {
        return (lhs.first == rhs.first) && (lhs.second == rhs.second);
    }

    bool _is_visitable(std::vector<std::vector<int>> const& map, std::pair<int, int> const& position)
    {
        if (!_is_out_of_range(map, position) &&
            !_is_no_go_area(map, position) &&
            !_is_visited(position))
            return true;

        return false;
    }

    std::pair<int, int> _add(std::pair<int, int> const& lhs, std::pair<int, int> const& rhs)
    {
        return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
    }

    std::pair<int, int> _subtract(std::pair<int, int> const& lhs, std::pair<int, int> const& rhs)
    {
        return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
    }

    std::pair<int, int> _absolute(std::pair<int, int> const& pair)
    {
        int x;
        int y;
        std::tie(x, y) = pair;

        if (x < 0)
            x *= -1;

        if (y < 0)
            y *= -1;

        return std::make_pair(x, y);
    }

    bool _is_evaluated(std::pair<int, int> const& position)
    {
        return std::get<2>(_costs[position.second][position.first]) > 0;
    }

    void _evaluate(std::pair<int, int> const& position)
    {
        int g = _calculate_value(_absolute(_subtract(position, _start)));
        int h = _calculate_value(_absolute(_subtract(position, _destination)));
        int f = g + h;

        _costs[position.second][position.first] = std::make_tuple(g, h, f);
    }

    int _calculate_value(std::pair<int, int> distance)
    {
        int ret = 0;
        while (distance.first > 0 &&
               distance.second > 0)
        {
            ret += 14;

            distance.first--;
            distance.second--;
        }

        if (distance.first > 0)
            ret += 10 * distance.first;
        else if (distance.second > 0)
            ret += 10 * distance.second;

        return ret;
    }

    void _make_priority_direction_factors(std::pair<int, int> const& from, std::pair<int, int> const& to)
    {
        auto direction = _subtract(to, from);
        auto absolute_direction = _absolute(direction);
        auto direction_x = 0;
        if (absolute_direction.first != 0)
            direction_x = direction.first / absolute_direction.first;
        auto direction_y = 0;
        if (absolute_direction.second != 0)
            direction_y = direction.second / absolute_direction.second;

        _direction_factors.resize(0);

        if (absolute_direction.first != 0 && absolute_direction.second != 0)
        {
            _direction_factors.emplace_back(std::make_pair(direction_x, direction_y));
            _direction_factors.emplace_back(std::make_pair(direction_x, 0));
            _direction_factors.emplace_back(std::make_pair(0, direction_y));
            _direction_factors.emplace_back(std::make_pair(direction_x, direction_y * -1));
            _direction_factors.emplace_back(std::make_pair(direction_x * -1, direction_y));
            _direction_factors.emplace_back(std::make_pair(direction_x * -1, 0));
            _direction_factors.emplace_back(std::make_pair(0, direction_y * -1));
            _direction_factors.emplace_back(std::make_pair(direction_x * -1, direction_y * -1));
        }
        else if (absolute_direction.first != 0)
        {
            _direction_factors.emplace_back(std::make_pair(direction_x, direction_y));
            _direction_factors.emplace_back(std::make_pair(direction_x, -1));
            _direction_factors.emplace_back(std::make_pair(direction_x, 1));
            _direction_factors.emplace_back(std::make_pair(0, -1));
            _direction_factors.emplace_back(std::make_pair(0, 1));
            _direction_factors.emplace_back(std::make_pair(direction_x * -1, -1));
            _direction_factors.emplace_back(std::make_pair(direction_x * -1, 1));
            _direction_factors.emplace_back(std::make_pair(direction_x * -1, direction_y * -1));
        }
        else
        {
            _direction_factors.emplace_back(std::make_pair(direction_x, direction_y));
            _direction_factors.emplace_back(std::make_pair(-1, direction_y));
            _direction_factors.emplace_back(std::make_pair(1, direction_y));
            _direction_factors.emplace_back(std::make_pair(-1, 0));
            _direction_factors.emplace_back(std::make_pair(1, 0));
            _direction_factors.emplace_back(std::make_pair(-1, direction_y * -1));
            _direction_factors.emplace_back(std::make_pair(1, direction_y * -1));
            _direction_factors.emplace_back(std::make_pair(direction_x * -1, direction_y * -1));
        }
    }

    std::vector<std::pair<int, int>> _direction_factors;
    std::vector<std::vector<bool>> _visits;
    std::vector<std::vector<std::tuple<int, int, int>>> _costs;
    std::pair<int, int> _start;
    std::pair<int, int> _destination;
};
