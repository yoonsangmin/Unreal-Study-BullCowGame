// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString &Word)
                                                    { return Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word); });

    // Isograms = GetValidWords(Words);

    // PrintLine(TEXT("%i"), Words.Num());
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString &PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // else Checking PlayerGuess
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    // Welcoming The Player
    PrintLine(TEXT("Hi Welcome to Bull Cows Game!"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; // Set the HiddenWord
    Lives = HiddenWord.Len();                                       // Set Lives
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess and\npress enter to continue...")); // Prompt Player For Guess

    PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord); // Debug Line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString &Guess)
{
    if (Guess == HiddenWord)

    {
        ClearScreen();

        PrintLine(TEXT("You have Won!"));

        EndGame();

        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        // PrintLine(TEXT("Sorry, try guessing again!\nYou have %i lives remaining"), Lives);
        PrintLine(TEXT("The hidden word is %i letters long, try again!"), HiddenWord.Len());

        return;
    }

    // Check If Isogram
    if (!IsIsogram(Guess))
    {
        /* code */
        PrintLine(TEXT("There're repeating letters, guess again!"));

        return;
    }

    // Remove Life
    --Lives;
    PrintLine(TEXT("Lost a life!"));

    // Check If Lives > 0
    if (Lives <= 0)
    {
        ClearScreen();

        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s."), *HiddenWord);

        EndGame();

        return;
    }

    // Show the player Bulls and Cows
    PrintLine(TEXT("Sorry, try guessing again!\nYou have %i lives remaining"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString &Word)
{
    for (int32 Index = 0; Index < Word.Len() - 1; Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8)
        {
            if (IsIsogram(Word))
            {
                ValidWords.Emplace(Word);
            }
        }
    }

    return ValidWords;
}