//
// Created by Mike Terentyev on 09/05/18.
//

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include "gtest.h"
#include "../serpent/decoder.h"
#include "../serpent/encoder.h"

using namespace std;

void commontest(const string& message, const string& key) {
    serpent::decoder dec(key);
    serpent::encoder enc(key);
    EXPECT_EQ(message, dec.decode(enc.encode(message)));
}


TEST(correctness, small1) {
    commontest("HI! LET'S TEST SERPENT SERPENT SERPENT", "cryptograhpyis<3");
}

TEST(correctness, small2) {
    commontest("Google Test. OSS Builds Status: Build status. Announcements: Release 1.10.x. Release 1.10.x is now available. Coming Soon. Post 1.10.x ", "OpenCppCoverageO");
}

TEST(correctness, small3) {
    commontest("Stroustrup has been a noble doctor at ITMO University since 2013.", "BjarneStroustrup");
}

TEST(correctness, big) {
    commontest("Stroustrup began developing C++ in 1979 (then called \"C with Classes\"), and, in his own words, \"invented C++, wrote its early definitions, and produced its first implementation... chose and formulated the design criteria for C++, designed all its major facilities, and was responsible for the processing of extension proposals in the C++ standards committee.\"[13] Stroustrup also wrote a textbook for the language in 1985, The C++ Programming Language.[14][15]\n"
               "\n"
               "Stroustrup was the head of AT&T Bell Labs' Large-scale Programming Research department, from its creation until late 2002. Stroustrup was elected member of the National Academy of Engineering in 2004. He was elected a Fellow of the Association for Computing Machinery (ACM) in 1994 and the Institute of Electrical and Electronics Engineers (IEEE).[when?] From 2002 to 2014, Stroustrup was the College of Engineering Chair in Computer Science Professor at Texas A&M University.[16][17] As of January 2014, Stroustrup is a Managing Director in the technology division of Morgan Stanley in New York City and a Visiting Professor in Computer Science at Columbia University.", "BjarneStroustrup");
}

TEST(correctness, bigbig) {
    commontest(R"(t was bright and sunny. A fine rain had been falling all the
morning, and now it had not long cleared up. The iron roofs, the
flags of the roads, the flints of the pavements, the wheels and
leather, the brass and the tinplate of the carriages--all
glistened brightly in the May sunshine. It was three o'clock,
and the very liveliest time in the streets.

As she sat in a corner of the comfortable carriage, that hardly
swayed on its supple springs, while the grays trotted swiftly, in
the midst of the unceasing rattle of wheels and the changing
impressions in the pure air, Anna ran over the events of the last
days, and she saw her position quite differently from how it had
seemed at home. Now the thought of death seemed no longer so
terrible and so clear to her, and death itself no longer seemed
so inevitable. Now she blamed herself for the humiliation to
which she had lowered herself. "I entreat him to forgive me. I
have given in to him. I have owned myself in fault. What for?
Can't I live without him?" And leaving unanswered the question
how she was going to live without him, she fell to reading the
signs on the shops. "Office and warehouse. Dental surgeon.
Yes, I'll tell Dolly all about it. She doesn't like Vronsky. I
shall be sick and ashamed, but I'll tell her. She loves me, and
I'll follow her advice. I won't give in to him; I won't let him
train me as he pleases. Filippov, bun shop. They say they send
their dough to Petersburg. The Moscow water is so good for it.
Ah, the springs at Mitishtchen, and the pancakes!"

And she remembered how, long, long ago, when she was a girl of
seventeen, she had gone with her aunt to Troitsa. "Riding, too.
Was that really me, with red hands? How much that seemed to me
then splendid and out of reach has become worthless, while what
I had then has gone out of my reach forever! Could I ever have
believed then that I could come to such humiliation? How
conceited and self-satisfied he will be when he gets my note!
But I will show him.... How horrid that paint smells! Why is it
they're always painting and building? Modes et robes," she read.
A man bowed to her. It was Annushka's husband. "Our parasites";
she remembered how Vronsky had said that. "Our? Why our?
What's so awful is that one can't tear up the past by its roots.
One can't tear it out, but one can hide one's memory of it. And
I'll hide it." And then she thought of her past with Alexey
Alexandrovitch, of how she had blotted the memory of it out of
her life. "Dolly will think I'm leaving my second husband, and
so I certainly must be in the wrong. As if I cared to be right!
I can't help it!" she said, and she wanted to cry. But at once
she fell to wondering what those two girls could be smiling
about. "Love, most likely. They don't know how dreary it is,
how low.... The boulevard and the children. Three boys running,
playing at horses. Seryozha! And I'm losing everything and not
getting him back. Yes, I'm losing everything, if he doesn't
return. Perhaps he was late for the train and has come back by
now. Longing for humiliation again!" she said to herself. "No,
I'll go to Dolly, and say straight out to her, I'm unhappy, I
deserve this, I'm to blame, but still I'm unhappy, help me.
These horses, this carriage--how loathsome I am to myself in this
carriage--all his; but I won't see them again."

Thinking over the words in which she would tell Dolly, and
mentally working her heart up to great bitterness, Anna went
upstairs.

"Is there anyone with her?" she asked in the hall.

"Katerina Alexandrovna Levin," answered the footman.

"Kitty! Kitty, whom Vronsky was in love with!" thought Anna,
"the girl he thinks of with love. He's sorry he didn't marry
her. But me he thinks of with hatred, and is sorry he had
anything to do with me.")", "KittyKittyVronsk");
}