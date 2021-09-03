#include "Simulator.h"

Simulator::Simulator()
{

}

void Simulator::giveOrder(Robot *)
{

}

unsigned Simulator::getScore()
{
   // Somme des énergies aspirées divisée par la somme de toutes les énergies produites
   // par le jeu. Cette dernière n’inclut pas les particules qui se sont décomposées mais
   // elle intègre celles qui se sont désintégrées
   return 0;
}
