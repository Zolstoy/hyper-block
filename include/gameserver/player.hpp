#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <string>

namespace hyper_block {

class player
{
   private:
    std::string nickname_;
    boost::numeric::ublas::vector<double> position_;
    bool facing_right_;
    bool is_moving_;

   public:
    std::string get_nickname() const;
    void set_nickname(const std::string& nickname);

    boost::numeric::ublas::vector<double> get_position() const;
    void set_position(const boost::numeric::ublas::vector<double>& position);

    bool is_facing_right() const;
    void set_facing_right(bool facing_right);

    bool is_moving() const;
    void set_moving(bool is_moving);
};
}   // namespace hyper_block
