#include"OAHashTable.h"
#include"Support.h" 
#include<cmath>
template<typename T>
OAHashTable<T>::OAHashTable(const OAHTConfig& Config):my_config(Config)
{
    my_stats.Count_ = 0;
    my_stats.Probes_ = 0;
    my_stats.Expansions_ = 0;
    my_stats.PrimaryHashFunc_ = my_config.PrimaryHashFunc_;
    my_stats.SecondaryHashFunc_ = my_config.SecondaryHashFunc_;
    my_stats.TableSize_ = GetClosestPrime(my_config.InitialTableSize_);


    my_slots=new OAHTSlot[my_stats.TableSize_];
    if (my_stats.SecondaryHashFunc_ != nullptr)
    {
        my_delete_policy = OAHTDeletionPolicy::MARK;
    }
}

template<typename T>
void OAHashTable<T>::InitTable()
{
    for (unsigned int i = 0; i < my_stats.TableSize_; i++)
    {
        my_slots[i].State = OAHTSlot::OAHTSlot_State::UNOCCUPIED;
    }
    my_stats.Count_ = 0;
}



//Destructor
template<typename T>
OAHashTable<T>::~OAHashTable()
{
    clear();
    delete my_slots;

}

//Insert
template<typename T>
void OAHashTable<T>::insert([[maybe_unused]]const char *Key, [[maybe_unused]]const T& Data)
{
     
}
//remove
template<typename T>
void OAHashTable<T>::remove([[maybe_unused]]const char *Key)
{
    
}

template<typename T>
void OAHashTable<T>::GrowTable()
{
    unsigned int new_table_size = std::ceil((my_stats.TableSize_ * my_config.GrowthFactor));
    new_table_size = GetClosestPrime(table_size);
    OHASlot* old_table = my_slot;
    unsigned int old_table_size = my_stats.TableSize_;
    my_slots = new OAHTSlot[table_size];
    if (my_slot == nullptr)
    {
        throw OAHashTableException(OAHASHTABLE_EXCEPTION);
    }
    my_stats.TableSize_=new_table_size
        for (unsigned int i = 0; i < my_stats.Tablesize_; i++)
        {
            insert
        }

}

template<typename T>
void OAHashTable<T>::calulate_load_factor()
{
    float current_load_factor;
    int  current_use_element=0;
    for (int i = 0; i < my_stats.TableSize_; i++)
    {
        if (my_slots[i].State == OAHTSlot::OAHTSlot_State::OCCUPIED)
        {
            current_use_element++;

        }
    }
    current_load_factor = static_cast<float>(current_use_element) / current_load_factor;
    return current_load_factor;

}




template<typename T>
const T& OAHashTable<T>::find([[maybe_unused]]const char *Key) const
{
    
}


template<typename T>
void OAHashTable<T>::clear()
{
  /*  if there is a free callback
        for each OCCUPIED slot
            free data
            Initialize Table*/
    if (my_config.FreeProc_ != nullptr)
    {
        for (int i = 0; i < my_stats.TableSize_; i++)
        {
            if (my_slots[i].State = OAHTSlot::OAHTSlot_State::OCCUPIED)
            {
                my_config.FreeProc_(my_slots[i].Data);
            }
        }
    }
    InitTable();


}

template<typename T>
OAHTStats  OAHashTable<T>::GetStats() const
{
    return my_stats;
}

template<typename T>
const typename  OAHashTable<T>::OAHTSlot* OAHashTable<T>::GetTable() const
{
    return my_slots;
}




