#include <stdio.h>
#include <cslice.h>
#include "song/song.h"
#include "common.h"

int main() {
    clear_data();
    fmt_i
    struct Song *stairway_to_heaven = song("Stairway to Heaven", "Led Zeppelin", "Stairway to Heaven lyrics here!!",
                                           483, 740000);

    struct Song *wind_of_change = song("Wind of Change", "The Scorpions", "I follow the Moskva, down to Gorky Park...",
                                       612,
                                       940000000);
    struct Song *hotel_california = song("Hotel California", "The Eagles", "Hotel California lyrics here!!", 432,
                                         57000000);

    struct Song *first = song("First", "Cold War Kids", "First lyrics here!!", 200, 45000000);

    struct Song *you_found_me = song("You Found Me", "The Fray", "You Found Me lyrics here!!", 482, 700000000);

    struct Song *paradise = song("Paradise", "Coldplay", "Paradise lyrics here!!", 369, 400000000);

    struct Song *viva_la_vida = song("Viva la Vida", "Coldplay", "Viva la Vida lyrics here", 242, 1100000000);

    struct Song *californication = song("Californication", "Red Hot Chili Peppers", "Californication lyrics here!!",
                                        330, 1050000000);

    struct Song *we_are_young = song("We Are Young", "Fun. and Janelle Monáe",
                                     "Give me a second I, need to get my story straight...", 251,
                                     1700000000);

    struct Song *somebody_else = song("Somebody Else", "The 1975", "Somebody Else lyrics here!!", 348, 1000000000);

    struct Song *tears_in_heaven = song("Tears in Heaven", "Eric Clapton",
                                        "Would you know my name? If I saw you in heaven?...", 272, 1100000000);
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
    write_song(tears_in_heaven);

    Slice *test = read_block(1);
    printf("%d\n", len(test));
    print(test, (void (*)(const void *)) serialize_song);


    return 0;
}
