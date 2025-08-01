#include "attack.hpp"
#include "types.hpp"
#include "position.hpp"
#include <iostream>



int main()
{
    Position pos;
    //print_board(pos.line_bb(c8, h3));

    pos.set_check_squares(WHITE);
    pos.set_check_squares(BLACK);
    pos.set_pins_info(WHITE);
    pos.set_pins_info(BLACK);

    print_board(pos.color_bb[WHITE]);
    print_board(pos.color_bb[BLACK]);

    std::cout << (pos.is_check() > 0 ? "YES IN CHECK" : " NOT IN CHECK");

    print_board(pos.set_checkers());

    std::cout << "BLOCKERS FOR KING";
    print_board(pos.state.blockers_for_king[WHITE]);
    print_board(pos.state.blockers_for_king[BLACK]);
    

    std::cout << "PINNERS FOR KING";
    print_board(pos.state.pinners[BLACK]);
    print_board(pos.state.pinners[WHITE]);

    return 0;
}