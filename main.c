#include <stdio.h>
#include <cslice.h>
#include "song/song.h"
#include "common.h"

int main() {
    clear_data();
    struct Song *stairway_to_heaven = song("Stairway to Heaven", "Led Zeppelin", "Stairway to Heaven lyrics here!!",
                                           483, 740000);

    struct Song *wind_of_change = song("Wind of Change", "The Scorpions", "Wind of Change lyrics here!!", 612,
                                       940000000);
    struct Song *hotel_california = song("Hotel California", "The Eagles", "Hotel California lyrics here!!", 432,
                                         57000000);

    struct Song *first = song("First", "Cold War Kids", "First lyrics here!!", 200, 45000000);

    struct Song *you_found_me = song("You Found Me", "The Fray", "You Found Me lyrics here!!", 482, 700000000);

    struct Song *paradise = song("Paradise", "Coldplay", "Paradise lyrics here!!", 369, 400000000);

    struct Song *viva_la_vida = song("Viva la Vida", "Coldplay", "Viva la Vida lyrics here", 242, 1100000000);

    struct Song *californication = song("Californication", "Red Hot Chili Peppers", "Californication lyrics here!!",
                                        330, 1050000000);

    struct Song *we_are_young = song("We Are Young", "Fun. and Janelle Monáe", "We Are Young lyrics here!!", 251,
                                     1700000000);

    struct Song *somebody_else = song("Somebody Else", "The 1975", "Somebody Else lyrics here!!", 348, 1000000000);


    write_song(stairway_to_heaven);
    write_song(wind_of_change);
    write_song(hotel_california);
    write_song(first);
    write_song(you_found_me);
    write_song(paradise);
    write_song(viva_la_vida);
    write_song(californication);
    write_song(we_are_young);
    write_song(somebody_else);


    Slice *test = read_block(0);
    printf("%d\n", len(test));
    print(test, (void (*)(const void *)) serialize_song);

    return 0;
}
