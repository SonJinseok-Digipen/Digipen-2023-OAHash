#include"OAHashTable.h"
#include<cmath>
template<typename T>
 OAHashTable<T>::OAHashTable(const OAHTConfig& Config):my_config(Config)
{
	 my_stats.Count_ = 0;
	 my_stats.Expansions_ = 0;
	 my_stats.PrimaryHashFunc_ = my_config.PrimaryHashFunc_;
	 my_stats.SecondaryHashFunc_ = my_config.SecondaryHashFunc_;
	 my_stats.Probes_ = 0;
	 my_stats.TableSize_ = 0;

	 my_stats.TableSize_ = GetClosestPrime(my_config.InitialTableSize_);
	 my_slots = new OAHTSlot[my_stats.TableSize_];
	 if (my_config.SecondaryHashFunc_ != nullptr)
	 {
		 my_config.DeletionPolicy_ = OAHTDeletionPolicy::MARK;
	 }
	 InitTable();
}

 template<typename T>
 void OAHashTable<T>::InitTable()
 {
	 for (int i = 0; i < my_stats.TableSize_; i++)
	 {
		 my_slots[i].State = OAHTSlot::UNOCCUPIED;
		
	 }
	 my_stats.Count_ = 0;
 }

 template<typename T>
 const T& OAHashTable<T>::find([[maybe_unused]]const char* Key) 
 {
	 OAHTSlot* slot;
	 int current_index=IndexOf(Key, slot);
	 if (current_index == -1)
	 {
		 throw OAHashTableException(OAHashTableException::OAHASHTABLE_EXCEPTION::E_ITEM_NOT_FOUND, "");
	 }
	 
	 return my_slots[current_index].Data;
	 
     
 }

 template<typename T>
 void OAHashTable<T>::clear()
 {
	 if (my_config.FreeProc_ != nullptr)
	 {
		 for (int i = 0; i < my_stats.TableSize_; i++)
		 {
			 my_config.FreeProc_(my_slots[i].Data);
		 }
	 }
	 InitTable();
	
 }

 template<typename T>
 OAHashTable<T>::~OAHashTable()
 {
	 clear();
	 delete[] my_slots;
 }

 template<typename T>
 float OAHashTable<T>::CalCulate_LoadFactor() const
 {
	 float current_use_item = 0;
	 for (int i = 0; i < my_stats.TableSize_; i++)
	 {
		 if (my_slots[i].State == OAHTSlot::OCCUPIED)
		 {
			 current_use_item++;
		 }
	 }
	 float current_load_factor = current_use_item / my_stats.TableSize_;
	 float rounded_num = round(current_load_factor * 100) / 100.0;
	 return rounded_num;
 }

 template<typename T>
 OAHTStats OAHashTable<T>::GetStats()
 {
	 return my_stats;
 }

 template<typename T>
 const typename OAHashTable<T>::OAHTSlot* OAHashTable<T>::GetTable() const
 {
	 return my_slots;
 }

 template<typename T>
 int OAHashTable<T>::IndexOf(const char* Key, OAHTSlot*& Slot)
 {

	 OAHTSlot* slot = nullptr;
	 unsigned int index = my_stats.PrimaryHashFunc_(Key,my_stats.TableSize_);
	 unsigned int start_index = index;
	 unsigned int stride = 0;
	 if (my_stats.SecondaryHashFunc_ != nullptr)
	 {
		 stride = 1 + my_stats.SecondaryHashFunc_(Key,my_stats.TableSize_ - 1);
	 }
	 else
	 {
		 stride = 1;
	 }
	 unsigned int slot_probe_count = 0;
	 bool is_done = false;
	 

	 OAHTSlot* current=nullptr;

	 
	 
	 while (is_done==false)
	 {
		 
		 my_stats.Probes_++;
		 slot_probe_count++;
		 current = &my_slots[index];

		 if ((current->State == OAHTSlot::DELETED) && slot == nullptr)
		 {
			 slot = current;
		 }
		 if (current->State == OAHTSlot::UNOCCUPIED)
		 {
			 if (slot == nullptr)
			 {
				 slot = current;
			 }
			 slot->probes = slot_probe_count;
			 Slot = slot;
			 return -1;
		 }
		 else if (current->State != OAHTSlot::DELETED)
		 {
			 if (strcmp(current->Key,Key)==0)
			 {
				 slot = current;
				 slot->probes = slot_probe_count;
				 Slot = slot;
				 return index;
			 }
		 }
		 
		 index = index + stride;
		 if (index >= my_stats.TableSize_)
		 {
			 index = 0;
		 }
		 if (index == start_index)
		 {
			 is_done = true;
		 }
	 }
	 Slot = current;
	 return -1;




 }


 template<typename T>
 void OAHashTable<T>::GrowTable()
 {
	 unsigned int new_size = std::ceil(my_stats.TableSize_ * my_config.GrowthFactor_);
	 new_size = GetClosestPrime(new_size);
	 OAHTSlot* old_slot = my_slots;
	 unsigned int old_table_size = my_stats.TableSize_;
	 my_slots = new OAHTSlot[new_size];
	 if (my_slots == nullptr)
	 {
		 throw OAHashTableException(OAHashTableException::OAHASHTABLE_EXCEPTION::E_NO_MEMORY, "Item not found in table.");
	 }
	 my_stats.TableSize_ = new_size;
	 InitTable();
	 for(int i=0; i<old_table_size; i++)
	 {
		 if (old_slot[i].State == OAHTSlot::OCCUPIED)
		 {
			 insert(old_slot[i].Key, old_slot[i].Data);
		 }
	 }
	 delete[] old_slot;
	 my_stats.Expansions_++;
	
	/* for each OCCUPIED slot in old table
		 insert(current key, current value)
		 delete old table
		 update number of expansion*/
 }

 template<typename T>
 void OAHashTable<T>::insert(const char* Key, const T& Data)
 {
	 float current_load_factor = CalCulate_LoadFactor();
	 if ((current_load_factor >= my_config.MaxLoadFactor_) || (my_stats.TableSize_ == my_stats.Count_))
	 {
		 GrowTable();
		 
		 
	 }

	 OAHTSlot* slot;
	 int current_index = IndexOf(Key, slot);
		/* if (current_index == -1)
		 {
			throw OAHashTableException(OAHashTableException::OAHASHTABLE_EXCEPTION::E_DUPLICATE, "");
		 }*/
		 slot->State = OAHTSlot::OCCUPIED;
		 std::memcpy(slot->Key, Key, sizeof(char) * MAX_KEYLEN);
		 slot->Data = Data;
		 my_stats.Count_++;
 }

 template<typename T>
 void OAHashTable<T>::remove([[maybe_unused]]const char* Key)
 {
	 OAHTSlot* slot;
	 int current_index = IndexOf(Key, slot);
	 if (current_index == -1)
	 {
		 throw OAHashTableException(OAHashTableException::OAHASHTABLE_EXCEPTION::E_ITEM_NOT_FOUND, "Key not in table.");
	 }
	 my_stats.Count_--;
	 if (my_config.FreeProc_ != nullptr)
	 {
		 my_config.FreeProc_(slot->Data);
	 }
	 if (my_config.DeletionPolicy_ == OAHTDeletionPolicy::MARK)
	 {
		 slot->State == OAHTSlot::DELETED;
	 }
	 else if(my_config.DeletionPolicy_ ==OAHTDeletionPolicy::PACK)
	 {
		 slot->State = OAHTSlot::UNOCCUPIED;
		 unsigned int start_index = current_index;
		 current_index++;
		 if (current_index >= my_stats.TableSize_)
		 {
			 current_index = 0;
			 
		 }
		 slot = &my_slots[current_index];
		 while (slot->State == OAHTSlot::OCCUPIED && current_index != start_index)
		 {
			 slot->State = OAHTSlot::UNOCCUPIED;
			 my_stats.Count_--;
			 char current_key[MAX_KEYLEN];
			 std::memcpy(current_key, slot->Key, sizeof(char) * MAX_KEYLEN);
			 insert(current_key, slot->Data);
			 current_index++;
			 if (current_index >= my_stats.TableSize_)
			 {
				 current_index = 0;

			 }
			 slot = &my_slots[current_index];
		 }
		 
	 }
 }
