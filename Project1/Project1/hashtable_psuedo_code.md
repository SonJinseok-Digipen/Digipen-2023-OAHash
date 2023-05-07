ctor
    copy config
    init stats
    table size <- GetClosestPrime to initial table size
    create table buffer with table size
    if there is a 2nd hash function
        delete policy must be MARK
    
    Initialize Table

Initialize Table
    for each slot in table
        set state to be UNOCCUPIED
    item count <- 0

dtor
    clear
    delete table

insert (key, data)
    calculate load factor
    GrowTable if load factor is bigger than max load factor or if item count is table size

    index, slot <- Get Index And Slot of key

    if index shows that key is already in table
        throw Duplicate exception
    slot State <- OCCUPIED
    slot Key <- key
    slot Data <- data

    update item count

GrowTable
    new size <- ceil of current table size multiply by growth factor
    new size <- GetClosestPrime to new size
    old table <- current table
    old table size <- current table size
    set table to buffer with new table size
    if failed to create new buffer
        throw NO MEMORY exception
    table size <- new size
    for each OCCUPIED slot in old table
        insert (current key, current value)
    delete old table
    update number of expansions

Get Index And Slot of key
    slot <- null
    index <- 1st hash of key with table size
    start index <- index
    
    if using 2nd hash
        stride <- 1 + 2nd hash of key with (table size - 1)
    else linear probing
        stride <- 1

    slot probe count <- 0

    while not done
        Update global probe count
        Update slot probe count
        current slot <- slot at index
        if current slot is DELETED and slot is null
            slot <- current slot
        if current slot is UNOCCUPIED
            if slot is null
                slot <- current slot
            save slot probe count to slot
            return {-1, slot}
        else if current slot is Not DELETED
            if current slot key equals key
                slot <- current slot
                save slot probe count to slot
                return {index, slot}
        index <- index + stride
        wrap around index if it is larger than table size
        if index equals start index
            done <- true

    return {-1, slot}
    


remove by key
    index, slot <- Get Index And Slot of key
    if index shows that the key is not in the table
        throw exception ITEM_NOT_FOUND
    
    update item count

    free slot data if given a free callback

    if MARK policy
        slot state <- DELETED
    else PACK policy
        slot state <- UNOCCUPIED
        start index <- index
        increment index
        wrap around index if it is larger than table size
        slot <- slot at new index
        
        while slot state is OCCUPIED and index is not start index
            slot state <- UNOCCUPIED
            update item count
            current key <- copy of slot key
            insert (current key, slot Data)
            increment index
            wrap around index if it is larger than table size
            slot <- slot at new index


find by key
    index, slot <- Get Index And Slot of key
    if index shows that the key is not in the table
        throw exception ITEM_NOT_FOUND
    return slot data

clear
    if there is a free callback
        for each OCCUPIED slot
            free data
    Initialize Table
