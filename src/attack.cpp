#include "attack.hpp"
#include "types.hpp"
#include <array>



std::array<std::array<Bitboard, NUM_SQUARES>, NUM_COLOR> AttackTables::pawn_attacks;
std::array<Bitboard, NUM_SQUARES> AttackTables::knight_attacks;
std::array<Bitboard, NUM_SQUARES> AttackTables::king_attacks;
std::array<std::array<Bitboard, ROOK_BLOCKER>, NUM_SQUARES> AttackTables::rook_attacks;
std::array<std::array<Bitboard, BISHOP_BLOCKER>, NUM_SQUARES> AttackTables::bishop_attacks;

static const std::array<Bitboard, NUM_SQUARES> rook_magics = 
{
    0xA180022080400230ULL, 0x0040100040022000ULL, 0x0080088020001002ULL, 0x0080080280841000ULL,
    0x4200042010460008ULL, 0x04800A0003040080ULL, 0x0400110082041008ULL, 0x008000A041000880ULL,
    0x10138001A080C010ULL, 0x0000804008200480ULL, 0x00010011012000C0ULL, 0x0022004128102200ULL,
    0x000200081201200CULL, 0x202A001048460004ULL, 0x0081000100420004ULL, 0x4000800380004500ULL,
    0x0000208002904001ULL, 0x0090004040026008ULL, 0x0208808010002001ULL, 0x2002020020704940ULL,
    0x8048010008110005ULL, 0x6820808004002200ULL, 0x0A80040008023011ULL, 0x00B1460000811044ULL,
    0x4204400080008EA0ULL, 0xB002400180200184ULL, 0x2020200080100380ULL, 0x0010080080100080ULL,
    0x2204080080800400ULL, 0x0000A40080360080ULL, 0x02040604002810B1ULL, 0x008C218600004104ULL,
    0x8180004000402000ULL, 0x488C402000401001ULL, 0x4018A00080801004ULL, 0x1230002105001008ULL,
    0x8904800800800400ULL, 0x0042000C42003810ULL, 0x008408110400B012ULL, 0x0018086182000401ULL,
    0x2240088020C28000ULL, 0x001001201040C004ULL, 0x0A02008010420020ULL, 0x0010003009010060ULL,
    0x0004008008008014ULL, 0x0080020004008080ULL, 0x0282020001008080ULL, 0x50000181204A0004ULL,
    0x48FFFE99FECFAA00ULL, 0x48FFFE99FECFAA00ULL, 0x497FFFADFF9C2E00ULL, 0x613FFFDDFFCE9200ULL,
    0xFFFFFFE9FFE7CE00ULL, 0xFFFFFFF5FFF3E600ULL, 0x0010301802830400ULL, 0x510FFFF5F63C96A0ULL,
    0xEBFFFFB9FF9FC526ULL, 0x61FFFEDDFEEDAEAEULL, 0x53BFFFEDFFDEB1A2ULL, 0x127FFFB9FFDFB5F6ULL,
    0x411FFFDDFFDBF4D6ULL, 0x0801000804000603ULL, 0x0003FFEF27EEBE74ULL, 0x7645FFFECBFEA79EULL,
};

static const std::array<Bitboard, NUM_SQUARES> bishop_magics = 
{
    0xFFEDF9FD7CFCFFFFULL, 0xFC0962854A77F576ULL, 0x5822022042000000ULL, 0x2CA804A100200020ULL,
    0x0204042200000900ULL, 0x2002121024000002ULL, 0xFC0A66C64A7EF576ULL, 0x7FFDFDFCBD79FFFFULL,
    0xFC0846A64A34FFF6ULL, 0xFC087A874A3CF7F6ULL, 0x1001080204002100ULL, 0x1810080489021800ULL,
    0x0062040420010A00ULL, 0x5028043004300020ULL, 0xFC0864AE59B4FF76ULL, 0x3C0860AF4B35FF76ULL,
    0x73C01AF56CF4CFFBULL, 0x41A01CFAD64AAFFCULL, 0x040C0422080A0598ULL, 0x4228020082004050ULL,
    0x0200800400E00100ULL, 0x020B001230021040ULL, 0x7C0C028F5B34FF76ULL, 0xFC0A028E5AB4DF76ULL,
    0x0020208050A42180ULL, 0x001004804B280200ULL, 0x2048020024040010ULL, 0x0102C04004010200ULL,
    0x020408204C002010ULL, 0x02411100020080C1ULL, 0x102A008084042100ULL, 0x0941030000A09846ULL,
    0x0244100800400200ULL, 0x4000901010080696ULL, 0x0000280404180020ULL, 0x0800042008240100ULL,
    0x0220008400088020ULL, 0x04020182000904C9ULL, 0x0023010400020600ULL, 0x0041040020110302ULL,
    0xDCEFD9B54BFCC09FULL, 0xF95FFA765AFD602BULL, 0x1401210240484800ULL, 0x0022244208010080ULL,
    0x1105040104000210ULL, 0x2040088800C40081ULL, 0x43FF9A5CF4CA0C01ULL, 0x4BFFCD8E7C587601ULL,
    0xFC0FF2865334F576ULL, 0xFC0BF6CE5924F576ULL, 0x80000B0401040402ULL, 0x0020004821880A00ULL,
    0x8200002022440100ULL, 0x0009431801010068ULL, 0xC3FFB7DC36CA8C89ULL, 0xC3FF8A54F4CA2C89ULL,
    0xFFFFFCFCFD79EDFFULL, 0xFC0863FCCB147576ULL, 0x040C000022013020ULL, 0x2000104000420600ULL,
    0x0400000260142410ULL, 0x0800633408100500ULL, 0xFC087E8E4BB2F736ULL, 0x43FF9E4EF4CA2C89ULL,
};

// relevant occupancy bits depending on square. (64 - relevant occupancy bits)
static const std::array<int, NUM_SQUARES> rook_shift = 
{
    52, 53, 53, 53, 53, 53, 53, 52, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    52, 53, 53, 53, 53, 53, 53, 52
};

static const std::array<int, NUM_SQUARES> bishop_shift = 
{
    58, 59, 59, 59, 59, 59, 59, 58, 
    59, 59, 59, 59, 59, 59, 59, 59, 
    59, 59, 57, 57, 57, 57, 59, 59, 
    59, 59, 57, 55, 55, 57, 59, 59, 
    59, 59, 57, 55, 55, 57, 59, 59, 
    59, 59, 57, 57, 57, 57, 59, 59, 
    59, 59, 59, 59, 59, 59, 59, 59, 
    58, 59, 59, 59, 59, 59, 59, 58
};


AttackTables::AttackTables() //NOLINT
{
    init_leaper_pieces();
    init_slider_pieces();
}

Bitboard AttackTables::gen_pawn_attacks(Color color, Square square)
{
    Bitboard attacks = 0ULL;
    Bitboard bitboard = 0ULL;

    set_bit(bitboard, square);

    if(color == WHITE)
    {
        // white pawn attacks
        attacks |= shift<NORTH_WEST>(bitboard);
        attacks |= shift<NORTH_EAST>(bitboard);
    }
    else 
    {
        // black pawns attacks
        attacks |= shift<SOUTH_WEST>(bitboard);
        attacks |= shift<SOUTH_EAST>(bitboard);
    }

    return attacks;
}

Bitboard AttackTables::gen_knight_attacks(Square square)
{
    Bitboard attacks = 0ULL;
    Bitboard bitboard = 0ULL;


    set_bit(bitboard, square);

    // north knight attacks
    attacks |= shift_knight<NE_LONG>(bitboard);
    attacks |= shift_knight<NW_LONG>(bitboard);
    attacks |= shift_knight<NE_SHORT>(bitboard);
    attacks |= shift_knight<NW_SHORT>(bitboard);

    attacks |= shift_knight<SW_SHORT>(bitboard);
    attacks |= shift_knight<SE_LONG>(bitboard);
    attacks |= shift_knight<SW_LONG>(bitboard);
    attacks |= shift_knight<SE_SHORT>(bitboard);

    return attacks;
}

Bitboard AttackTables::gen_king_attacks(Square square)
{
    Bitboard attacks = 0ULL;
    Bitboard bitboard = 0ULL;

    set_bit(bitboard, square);

    // east attacks
    
    attacks |= shift<NORTH>(bitboard);
    attacks |= shift<SOUTH>(bitboard);

    attacks |= shift<WEST>(bitboard);
    attacks |= shift<EAST>(bitboard);

    attacks |= shift<NORTH_WEST>(bitboard);
    attacks |= shift<NORTH_EAST>(bitboard);

    attacks |= shift<SOUTH_WEST>(bitboard);
    attacks |= shift<SOUTH_EAST>(bitboard);

    return attacks;
}

void AttackTables::init_leaper_pieces()
{
    for(int sq = 0; sq < NUM_SQUARES; sq++)
    {
        king_attacks[sq] = gen_king_attacks(sq);
        knight_attacks[sq] = gen_knight_attacks(sq);

        for(int side = 0; side < NUM_COLOR; side++)
        {
            pawn_attacks[side][sq] = gen_pawn_attacks(side, sq);
        }
    }
}

Bitboard AttackTables::mask_rook_attacks(Square square) 
{
    Bitboard rook_mask = 0ULL;

    //rank file
    int rank = 0;
    int file = 0;

    int target_rank = square / NUM_RANKS;
    int target_file = square % NUM_FILES;


    // generate the attack mask excluding walls rank and files as they do not restrict movement
    // north
    for(rank = target_rank + 1; rank < RANK_8; rank++) { rook_mask |= 1ULL << (rank * NUM_RANKS + target_file); }
    // south
    for(rank = target_rank - 1; rank > RANK_1; rank--) { rook_mask |= 1ULL << (rank * NUM_RANKS + target_file); }
    // east
    for(file = target_file + 1; file < FILE_H; file++) { rook_mask |= 1ULL << (target_rank * NUM_RANKS + file); }
    // west
    for(file = target_file - 1; file > FILE_A; file--) { rook_mask |= 1ULL << (target_rank * NUM_RANKS + file); }

    return rook_mask;
}

Bitboard AttackTables::mask_bishop_attacks(Square square) 
{
    Bitboard bishop_mask = 0ULL;

    // rank file
    int rank = 0;
    int file = 0;

    int target_rank = square / NUM_RANKS;
    int target_file = square % NUM_FILES;

    // north east
    for(rank = target_rank + 1, file = target_file + 1; rank < RANK_8 && file < FILE_H; rank++, file++) { bishop_mask |= 1ULL << (rank * NUM_RANKS + file); }
    // south west
    for(rank = target_rank - 1, file = target_file - 1; rank > RANK_1 && file > FILE_A; rank--, file--) { bishop_mask |= 1ULL << (rank * NUM_RANKS + file); }
    // north west
    for(rank = target_rank + 1, file = target_file - 1; rank < RANK_8 && file > FILE_A; rank++, file--) { bishop_mask |= 1ULL << (rank * NUM_RANKS + file); }
    // south east
    for(rank = target_rank - 1, file = target_file + 1; rank > RANK_1 && file < FILE_H; rank--, file++) { bishop_mask |= 1ULL << (rank * NUM_RANKS + file); }

    return bishop_mask;
}

Bitboard AttackTables::gen_occupancy(int index, Bitboard attack_mask)
{
    Bitboard occupancy = 0ULL;
    Bitboard attack_copy = attack_mask; // we need mask later on in generating the magic index
    
    int bits_in_mask = bit_count(attack_copy);

    for(int count = 0; count < bits_in_mask; count++)
    {
        int square = lsb(attack_copy);
        pop_bit(attack_copy, square);

        if((index & (1 << count)) != 0){
            occupancy |= (1ULL << square);
        }
    }
    return occupancy;
}


Bitboard AttackTables::gen_rook_attacks_onfly(Square square, Bitboard occupancy)
{   
    Bitboard attacks = 0ULL;;
    int rank = 0;
    int file = 0;
    
    // current rank and file;
    int target_rank = square / NUM_RANKS;
    int target_file = square % NUM_FILES;
    // generate rooks with all attacks possible and check if the square is occupied then stop, include edges since this is entire attack

    // north
    for(rank = target_rank + 1; rank <= RANK_8; rank++) 
    {
        attacks |= 1ULL << (rank * NUM_RANKS + target_file);
        if(((1ULL << (rank * NUM_RANKS + target_file)) & occupancy)) { break; }
    }

    // south
    for(rank = target_rank - 1; rank >= RANK_1; rank--) 
    {
        attacks |= 1ULL << (rank * NUM_RANKS + target_file);
        if(((1ULL << (rank * NUM_RANKS + target_file)) & occupancy) != 0) { break; }
    }
    // east
    for(file = target_file + 1; file <= FILE_H; file++) 
    {
        attacks |= 1ULL << (target_rank * NUM_RANKS + file);
        if(((1ULL << (target_rank * NUM_RANKS + file)) & occupancy)) { break; }
    }
    // west
    for(file = target_file - 1; file >= (FILE_A); file--) 
    {
        attacks |= 1ULL << (target_rank * NUM_RANKS + file);
        if(((1ULL << (target_rank * NUM_RANKS + file)) & occupancy) != 0) { break; }
        
    }
    return attacks;
}

Bitboard AttackTables::gen_bishop_attacks_onfly(Square square, Bitboard occupancy)
{
    Bitboard attacks = 0ULL;

    int rank = 0;
    int file = 0;

    int target_rank = square / NUM_RANKS;
    int target_file = square % NUM_FILES;

    // north east
    for (rank = target_rank + 1, file = target_file + 1; rank <= RANK_8 && file <= FILE_H; rank++, file++) 
    {
        attacks |= (1ULL << (rank * NUM_RANKS + file ));
        if ((occupancy & (1ULL << (rank * NUM_RANKS + file))) != 0) { break; }
    }
    // south west
    for (rank = target_rank - 1, file = target_file - 1; rank >= RANK_1 && file >= FILE_A; rank--, file--) 
    {
        attacks |= (1ULL << (rank * NUM_RANKS + file ));
        if ((occupancy & (1ULL << (rank * NUM_RANKS + file))) != 0) { break; }
    }
    // north west
    for (rank = target_rank + 1, file = target_file - 1; rank <= RANK_8 && file >= FILE_A; rank++, file--) 
    {
        attacks |= (1ULL << (rank * NUM_RANKS + file ));
        if ((occupancy & (1ULL << (rank * NUM_RANKS + file))) != 0) { break; }
    }
    // south east
    for (rank = target_rank - 1, file = target_file + 1; rank >= RANK_1 && file <= FILE_H; rank--, file++) 
    {
        attacks |= (1ULL << (rank * NUM_RANKS + file ));
        if ((occupancy & (1ULL << (rank * NUM_RANKS + file))) != 0) { break; }
    }

    return attacks;
}


void AttackTables::init_slider_pieces()
{
    for(Square square = 0; square < NUM_SQUARES; square++)
    {
        Bitboard mask = mask_rook_attacks(square);
        int bits = bit_count(mask);
    
        for(int i = 0; i < (1 << bits); i++)
        {
            Bitboard occupancy = gen_occupancy(i, mask);
            Bitboard attacks = gen_rook_attacks_onfly(square, occupancy);

            int index = (occupancy * rook_magics[square]) >> rook_shift[square];
            rook_attacks[square][index] = attacks;
        }    
    }


    for(Square square = 0; square < NUM_SQUARES; square++)
    {
        Bitboard mask = mask_bishop_attacks(square);
        int bits = bit_count(mask);

        for(int i = 0; i < (1 << bits); i++)
        {
            Bitboard occupancy = gen_occupancy(i, mask);
            Bitboard attacks = gen_bishop_attacks_onfly(square, occupancy);

            Bitboard index = (occupancy * bishop_magics[square]) >> bishop_shift[square];
            bishop_attacks[square][index] = attacks;
        }    
    }
}




Bitboard AttackTables::get_rook_attacks(Square square, Bitboard occupancy)
{  
    occupancy &= mask_rook_attacks(square);  
    int index = (occupancy * rook_magics[square]) >> rook_shift[square]; 
    return rook_attacks[square][index];  
}  

Bitboard AttackTables::get_bishop_attacks(Square square, Bitboard occupancy)
{    
    occupancy &= mask_bishop_attacks(square);  
    int index = (occupancy * bishop_magics[square]) >> bishop_shift[square];  
    return bishop_attacks[square][index];  
}  
  
Bitboard AttackTables::get_queen_attacks(Square square, Bitboard occupancy)
{  
    return get_rook_attacks(square, occupancy) | get_bishop_attacks(square, occupancy);  
}