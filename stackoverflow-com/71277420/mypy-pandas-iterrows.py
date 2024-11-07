import pandas
import typing
from typing import Optional


def some() -> None:
    someTable = pandas.DataFrame(
        {
            "a": pandas.Series(dtype="float"),
            "b": pandas.Series(dtype="float"),
            "c": pandas.Series(dtype="str"),
        }
    )
    #someTable: pandas.DataFrame = pandas.DataFrame()

    for x in [1,2]:
        some = []
        for y in [3,4,5]:
            some.append(x * y)
        someRow = pandas.DataFrame(
            {
                "a": [some[0]],
                "b": [some[1]],
                "c": ["ololo"]
            },
            index = [x]
        )
        someTable = pandas.concat([someTable, someRow])

    #print(someTable)

    for index, row in someTable.iterrows():
        # reveal_type(index)
        print(type(index))
        index = typing.cast(int, index)
        print(index + 1)


some()
