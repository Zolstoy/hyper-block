#include "player.hpp"

namespace hyper_block {
std::string
player::get_nickname() const
{
    return nickname_;
}
void
player::set_nickname(const std::string& nickname)
{
    nickname_ = nickname;
}
boost::numeric::ublas::vector<double>
player::get_position() const
{
    return position_;
}
void
player::set_position(const boost::numeric::ublas::vector<double>& position)
{
    position_ = position;
}
bool
player::is_facing_right() const
{
    return facing_right_;
}
void
player::set_facing_right(bool facing_right)
{
    facing_right_ = facing_right;
}
bool
player::is_moving() const
{
    return is_moving_;
}
void
player::set_moving(bool is_moving)
{
    is_moving_ = is_moving;
}
}   // namespace hyper_block
