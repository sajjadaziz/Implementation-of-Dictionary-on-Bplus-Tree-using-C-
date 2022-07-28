#include<iostream>
#include<conio.h>
#include<fstream>
using namespace std;
int order = 28;
class node
{
	private:
	string *word;
	string *meaning;
	string *synonym;
	string *antonym;
	int size;
	node **child;
	bool leaf;
	friend class BPTREE;
	public:
	node()
	{
		size = 0;
		word = new string[order-1];
		meaning = new string[order-1];
		synonym = new string[order-1];
		antonym = new string[order-1];
		child = new node *[order];
	}
};
class BPTREE
{
	public:
		node *root;
		BPTREE()
		{
			root = NULL;
		}
		void reading_from_file()
		{
			fstream obj;
			string word1, word2, word3, word4;
			obj.open("Words.txt");
			if( ! obj.is_open() )
			{
				cout<<"File doesn't exist...!";
				exit(0);
			}
			while( getline(obj, word1) )
			{
				getline(obj, word2);
				getline(obj, word3);
				getline(obj, word4);
				insert(word1, word2, word3, word4);
			}
		}
		void insert(string w, string m, string s, string a)
		{
			if(root == NULL)
			{
				root = new node;
				root->word[0] = w;
				root->meaning[0] = m;
				root->synonym[0] = s;
				root->antonym[0] = a;
				root->size = 1;
				root->leaf = 1;
			}
			else
			{
				node *current, *top_root;
				current = root;
                while(current->leaf != 1)
				{
                    top_root = current;
                    for(int i=0 ; i<current->size ; i++)
					{
                        if(w < current->word[i])
						{
							current = current->child[i];
							break;
						}
                        if(i == current->size-1)
						{
							current = current->child[i+1];
							break;
						}
                    }
                }
                if(current->size < (order-1))
				{
					insert_data(w, m, s, a, current);
				}
				else
				{
					insert_data_in_new_node(w, m, s, a, current, top_root);
				}
            }
		}
	private:
		void insert_data(string w, string m, string s, string a, node *current)
		{
			int i=0, j=0;
			i=check_order(w, current);
			if(current->size>i)
			{
				re_order(i, current);
			}
            current->word[i] = w;
            current->meaning[i] = m;
            current->synonym[i] = s;
            current->antonym[i] = a;
            current->size++;
            current->child[current->size] = current->child[current->size - 1];
            current->child[current->size - 1] = NULL;
		}
		void insert_data_in_new_node(string w, string m, string s, string a, node *current, node *top_root)
		{
			node *new_node = new node;
			string new_word[order];
			string new_meaning[order];
			string new_synonym[order];
			string new_antonym[order];
			int i=0, j=0;
            for(i=0 ; i<(order-1) ; i++)
			{
				new_word[i] = current->word[i];
				new_meaning[i] = current->meaning[i];
				new_synonym[i] = current->synonym[i];
				new_antonym[i] = current->antonym[i];
			}
            i=0;
            while(w>new_word[i] && i<(order-1))
            {
            	i++;
			}
            for(j=order-1 ; j>i ; j--)
			{
				new_word[j] = new_word[j-1];
				new_meaning[j] = new_meaning[j-1];
				new_synonym[j] = new_synonym[j-1];
				new_antonym[j] = new_antonym[j-1];
			}
            new_word[i] = w;
            new_meaning[i] = m;
            new_synonym[i] = s;
            new_antonym[i] = a;
            current->size = order/2;
            new_node->size = order - current->size;
            new_node->leaf = 1;
            current->child[current->size] = new_node;
            new_node->child[new_node->size] = current->child[order-1];
            current->child[order-1] = NULL;
            for(i=0 ; i<current->size ; i++)
			{
				current->word[i] = new_word[i];
				current->meaning[i] = new_meaning[i];
				current->synonym[i] = new_synonym[i];
				current->antonym[i] = new_antonym[i];
			}
            for(i=0, j=current->size ; i<new_node->size ; i++, j++)
			{
				new_node->word[i] = new_word[j];
				new_node->meaning[i] = new_meaning[j];
				new_node->synonym[i] = new_synonym[j];
				new_node->antonym[i] = new_antonym[j];
			}
			i=0;
            if(current == root)
			{
                node *new_root = new node;
                new_root->word[i] = new_node->word[i];
                new_root->meaning[i] = new_node->meaning[i];
                new_root->synonym[i] = new_node->synonym[i];
                new_root->antonym[i] = new_node->antonym[i];
                new_root->child[i] = current;
                new_root->child[i+1] = new_node;
                new_root->leaf = 0;
                new_root->size = 1;
                root = new_root;
            }
			else
			{
                insert_in_top_root(new_node->word[0], new_node->meaning[0], new_node->synonym[0], new_node->antonym[0], top_root, new_node);
            }
		}
		void insert_in_top_root(string w, string m, string s, string a, node *current, node *new_node)
		{
			if(current->size < (order-1))
			{
                int i=0, j=0;
                i=check_order(w, current);
                if(current->size>i)
			    {
				    re_order(i, current);
			    }
                for(j=current->size+1 ; j>i+1 ; j--)
				{
					current->child[j] = current->child[j-1];
				}
                current->word[i] = w;
                current->meaning[i] = m;
                current->synonym[i] = s;
                current->antonym[i] = a;
                current->size++;
                current->child[i+1] = new_node;
            }
			else
            {
            	node *new_node2 = new node;
            	node *new_child[order+1];
            	string new_word[order];
            	string new_meaning[order];
            	string new_synonym[order];
            	string new_antonym[order];
            	int i=0, j=0;
                for(i=0 ; i<(order-1) ; i++)
				{
					new_word[i] = current->word[i];
					new_meaning[i] = current->meaning[i];
					new_synonym[i] = current->synonym[i];
					new_antonym[i] = current->antonym[i];
				}
                for(i=0 ; i<order ; i++)
				{
					new_child[i] = current->child[i];
				}
				i=0;
                while(w>new_word[i] && i < (order-1))
                {
                	i++;
				}
                for(j=order ; j>i ; j--)
                {
                	new_word[j] = new_word[j-1];
                	new_meaning[j] = new_meaning[j-1];
                	new_synonym[j] = new_synonym[j-1];
                	new_antonym[j] = new_antonym[j-1];
				}
                new_word[i] = w;
                for(j=order+1 ; j>i+1 ; j--)
				{
					new_child[j] = new_child[j - 1];
				}
                new_child[i+1] = new_node;
                new_node2->leaf = 0;
                current->size = order/2;
                new_node2->size = (order-1) - current->size;
                for(i=0, j=current->size+1 ; i<new_node2->size ; i++, j++)
				{
					new_node2->word[i] = new_word[j];
					new_node2->meaning[i] = new_meaning[j];
					new_node2->synonym[i] = new_synonym[j];
					new_node2->antonym[i] = new_antonym[j];
				}
                for(i=0, j=current->size+1 ; i<new_node2->size+1 ; i++, j++)
				{
					new_node2->child[i] = new_child[j];
				}
				i=0;
                if (current == root)
				{
					node *new_root = new node;
					new_root->word[0] = current->word[current->size];
					new_root->meaning[0] = current->meaning[current->size];
					new_root->synonym[0] = current->synonym[current->size];
					new_root->antonym[0] = current->antonym[current->size];
                    new_root->child[0] = current;
                    new_root->child[1] = new_node2;
                    new_root->leaf = 0;
                    new_root->size = 1;
                    root = new_root;
				}
                else
				{
                    insert_in_top_root(current->word[current->size], current->meaning[current->size], current->synonym[current->size], current->antonym[current->size], find_parent(root, current), new_node2);
                }
		    }
	    }
	    int check_order(string w, node *current)
	    {
	    	int i=0;
	    	while(w>current->word[i] && i<current->size)
            {
            	i++;
			}
			return i;
		}
		void re_order(int i, node *current)
		{
			for(int j=current->size ; j>i ; j--)
			{
				current->word[j] = current->word[j-1];
            	current->meaning[j] = current->meaning[j-1];
            	current->synonym[j] = current->synonym[j-1];
            	current->antonym[j] = current->antonym[j-1];
			}
		}
		node* find_parent(node *root, node *current)
		{
    		node *new_root;
    		if(root->leaf || root->child[0]->leaf)
    		{
    		    return NULL;
    		}
    		for(int i = 0 ; i<root->size+1 ; i++)
			{
        		if(root->child[i] == current)
				{
        		    new_root = root;
            		return new_root;
        		}
				else
				{
            		new_root = find_parent(root->child[i], current);
            		if (new_root != NULL)
            		{
            			return new_root;
					}
        		}
    		}
    		return new_root;
		}
	public:
		void remove(string w)
		{
			int i, j;
			if(root==NULL)
			{
				cout<<"\n\nThe tree is empty...!\n";
				return ;
			}
			else
			{
				node *current, *top_root;
				int right=0, left=0;
				current = root;
				while(current->leaf == 0)
				{
					for(i=0 ; i<current->size ; i++)
					{
						top_root = current;
						left = i-1;
						right = i+1;
						if(w < current->word[i])
						{
							current = current->child[i];
							break;
						}
						if(i == current->size - 1)
						{
							right = i+2;
							left = i;
							current = current->child[i+1];
							break;
						}
					}
				}
				bool found = 0;
				for(i=0 ; i<current->size ; i++)
				{
					if(current->word[i] == w)
					{
						found = 1;
						break;
					}
				}
				if(found==0)
				{
					cout<<"\n\n\n\n\n\n\t\t\t\t\t\t\tThe word is not found...!";
					return;
				}
				for(int j=i ; j<current->size-1 ; j++)
				{
					current->word[j] = current->word[j+1];
				    current->meaning[j] = current->meaning[j+1];
				    current->synonym[j] = current->synonym[j+1];
				    current->antonym[j] = current->antonym[j+1];
				}
				current->size--;
				if(current == root)
				{
					for(int i=0 ; i<order+1 ; i++)
					    current->child[i] = NULL;
					if(current->size == 0)
					{
						delete[] current->word;
						delete[] current->meaning;
						delete[] current->synonym;
						delete[] current->antonym;
						delete[] current->child;
						delete current;
						root = NULL;
					}
					cout<<"\n\n\n\n\n\t\t\t\t\t\t\t"<<w<<" is successfully removed...!\n\n";
					return ;
				}
				current->child[current->size] = current->child[current->size+1];
				current->child[current->size+1] = NULL;
				if(current->size >= (order+1)/2 )
			    {
				    cout<<"\n\n\n\n\n\t\t\t\t\t\t\t"<<w<<" is successfully removed...!\n\n";
				    return ;
				}
				if(left >= 0)
				{
					node *left_node = top_root->child[left];
					if(left_node->size >= ((order+1)/2)+1);
					{
						for(int i=current->size ; i>0 ; i--)
						{
							current->word[i] = current->word[i-1];
							current->meaning[i] = current->meaning[i-1];
							current->synonym[i] = current->synonym[i-1];
							current->antonym[i] = current->antonym[i-1];
						}
						current->size++;
						current->child[current->size] = current->child[current->size-1];
						current->child[current->size-1] = NULL;
						current->word[0] = left_node->word[left_node->size-1];
						current->meaning[0] = left_node->meaning[left_node->size-1];
						current->synonym[0] = left_node->synonym[left_node->size-1];
						current->antonym[0] = left_node->antonym[left_node->size-1];
						left_node->size--;
						left_node->child[left_node->size] = current;
						left_node->child[left_node->size+1] = NULL;
						top_root->word[left] = current->word[0];
						top_root->meaning[left] = current->meaning[0];
						top_root->synonym[left] = current->synonym[0];
						top_root->antonym[left] = current->antonym[0];
					    cout<<"\n\n\n\n\n\t\t\t\t\t\t\t"<<w<<" is successfully removed...!\n\n";
						return ;
					}
				}
				if(right <= top_root->size)
				{
					node *right_node = top_root->child[right];
					if(right_node->size>=((order+1)/2)+1)
					{
						current->size++;
					    current->child[current->size] = current->child[current->size-1];
					    current->child[current->size-1] = NULL;
					    current->word[current->size-1] = right_node->word[0];
					    current->meaning[current->size-1] = right_node->meaning[0];
					    current->synonym[current->size-1] = right_node->synonym[0];
					    current->antonym[current->size-1] = right_node->antonym[0];
					    right_node->size--;
					    right_node->child[right_node->size] = right_node->child[right_node->size+1];
					    right_node->child[right_node->size+1] = NULL;
					    for(int i=0 ; i<right_node->size ; i++)
					    {
					    	right_node->word[i] = right_node->word[i+1];
					    	right_node->meaning[i] = right_node->meaning[i+1];
					    	right_node->synonym[i] = right_node->synonym[i+1];
					    	right_node->antonym[i] = right_node->antonym[i+1];
						}
						top_root->word[right-1] = right_node->word[0];
						top_root->meaning[right-1] = right_node->word[0];
						top_root->synonym[right-1] = right_node->synonym[0];
						top_root->antonym[right-1] = right_node->antonym[0];
					    cout<<"\n\n\n\n\n\t\t\t\t\t\t\t"<<w<<" is successfully removed...!\n\n";
						return ;
					}
				}
				if(left >= 0)
				{
					node *left_node = top_root->child[left];
					for(int i=left_node->size, j=0 ; j<current->size ; i++, j++)
					{
						left_node->word[i] = current->word[j];
						left_node->meaning[i] = current->meaning[j];
						left_node->synonym[i] = current->synonym[j];
						left_node->antonym[i] = current->antonym[j];
					}
					left_node->child[left_node->size] = NULL;
					left_node->size += current->size;
					left_node->child[left_node->size] = current->child[current->size];
					remove_from_top_root(top_root->word[left], top_root, current);
					delete[] current->word;
					delete[] current->meaning;
					delete[] current->synonym;
					delete[] current->antonym;
					delete[] current->child;
					delete current;
				}
				else if(right<=top_root->size)
				{
					node *right_node = top_root->child[right];
					for(int i=current->size, j=0 ; j<right_node->size ; i++, j++)
					{
						current->word[i] = right_node->word[j];
						current->meaning[i] = right_node->meaning[j];
						current->synonym[i] = right_node->synonym[j];
						current->antonym[i] = right_node->antonym[j];
					}
					current->child[current->size] = NULL;
					current->size += right_node->size;
					current->child[current->size] = right_node->child[right_node->size];
					remove_from_top_root(top_root->word[right-1], top_root, right_node);
					delete[] right_node->word;
					delete[] right_node->meaning;
					delete[] right_node->synonym;
					delete[] right_node->antonym;
					delete[] right_node->child;
					delete right_node;
				}
			}
		}
		private:
		void remove_from_top_root(string w, node *current, node *temp)
		{
			int i, j;
			if(current == root)
			{
				if(current->size == 1)
				{
					if(current->child[1] == temp)
					{
						delete[] temp->word;
						delete[] temp->meaning;
						delete[] temp->synonym;
						delete[] temp->antonym;
						delete[] temp->child;
						delete temp;
						root = current->child[0];
						delete[] current->word;
						delete[] current->meaning;
						delete[] current->synonym;
						delete[] current->antonym;
						delete[] current->child;
						delete current;
					    cout<<"\n\n\n\n\n\t\t\t\t\t\t\t"<<w<<" is successfully removed...!\n\n";
						return ;
					}
					else if(current->child[0] == temp)
					{
						delete[] temp->word;
						delete[] temp->meaning;
						delete[] temp->synonym;
						delete[] temp->antonym;
						delete[] temp->child;
						delete temp;
						root = current->child[1];
						delete[] current->word;
						delete[] current->meaning;
						delete[] current->synonym;
						delete[] current->antonym;
						delete[] current->child;
						delete current;
					    cout<<"\n\n\n\n\n\t\t\t\t\t\t\t"<<w<<" is successfully removed...!\n\n";
						return ;
					}
				}
			}
			for(i=0 ; i<current->size ; i++)
			{
				if(current->word[i] == w)
				break;
			}
			for(j=i ; j<current->size ; j++)
			{
				current->word[j] = current->word[j+1];
				current->meaning[j] = current->meaning[j+1];
				current->synonym[j] = current->synonym[j+1];
				current->antonym[j] = current->antonym[j+1];
			}
			for(i=0 ; i<current->size+1 ; i++)
			{
				if(current->child[i] == temp)
				break;
			}
			for(j=i ; j<current->size+1 ; j++)
			    current->child[j] = current->child[j+1];
			current->size--;
			if(current->size>=((order+1)/2)-1)
			{
				cout<<"\n\n\n\n\n\t\t\t\t\t\t\t"<<w<<" is successfully removed...!\n\n";
				return ;
			}
			if(current == root)
			{
				cout<<"\n\n\n\n\n\t\t\t\t\t\t\t"<<w<<" is successfully removed...!\n\n";
				return ;
			}
			node *top_root = find_parent(root, current);
			int left, right, pos;
			for(pos=0 ; pos<top_root->size+1 ; pos++)
			{
				if(top_root->child[pos] == current)
				{
					left = pos-1;
					right = pos+1;
					break;
				}
			}
			if(left >= 0)
			{
				node *left_node = top_root->child[left];
				if(left_node->size >= (order+1)/2)
				{
					for(i=current->size ; i>0 ; i--)
					{
						current->word[i] = current->word[i-1];
						current->meaning[i] = current->meaning[i-1];
						current->synonym[i] = current->synonym[i-1];
						current->antonym[i] = current->antonym[i-1];
					}
					current->word[0] = top_root->word[left];
					current->meaning[0] = top_root->meaning[left];
					current->synonym[0] = top_root->synonym[left];
					current->antonym[0] = top_root->antonym[left];
					top_root->word[left] = left_node->word[left_node->size-1];
					top_root->meaning[left] = left_node->meaning[left_node->size-1];
					top_root->synonym[left] = left_node->synonym[left_node->size-1];
					top_root->antonym[left] = left_node->antonym[left_node->size-1];
					for(i=current->size+1 ; i>0 ; i--)
					    current->child[i] = current->child[i-1];
					current->child[0] = left_node->child[left_node->size];
					current->size++;
					left_node->size--;
					cout<<"\n\n\n\n\n\t\t\t\t\t\t\t"<<w<<" is successfully removed...!\n\n";
					return ;
				}
			}
			if(right <= top_root->size)
			{
				node *right_node = top_root->child[right];
				if(right_node->size >= (order+1)/2)
				{
					current->word[current->size] = top_root->word[pos];
					current->meaning[current->size] = top_root->meaning[pos];
					current->synonym[current->size] = top_root->synonym[pos];
					current->antonym[current->size] = top_root->antonym[pos];
					top_root->word[pos] = right_node->word[0];
					top_root->meaning[pos] = right_node->meaning[0];
					top_root->synonym[pos] = right_node->synonym[0];
					top_root->antonym[pos] = right_node->antonym[0];
					for(i=0 ; i<right_node->size-1 ; i++)
					{
						right_node->word[i] = right_node->word[i+1];
						right_node->meaning[i] = right_node->meaning[i+1];
						right_node->synonym[i] = right_node->synonym[i+1];
						right_node->antonym[i] = right_node->antonym[i+1];
					}
					current->child[current->size+1] = right_node->child[0];
					for(i=0 ; i<right_node->size ; ++i)
					    right_node->child[i] = right_node->child[i+1];
					current->size++;
					right_node->size--;
					cout<<"\n\n\n\n\n\t\t\t\t\t\t\t"<<w<<" is successfully removed...!\n\n";
					return ;
				}
			}
			if(left >= 0)
			{
				node *left_node = top_root->child[left];
				left_node->word[left_node->size] = top_root->word[left];
				left_node->meaning[left_node->size] = top_root->meaning[left];
				left_node->synonym[left_node->size] = top_root->synonym[left];
				left_node->antonym[left_node->size] = top_root->antonym[left];
				for(i=left_node->size+1, j=0 ; i<current->size ; j++)
				{
					left_node->word[i] = current->word[j];
					left_node->meaning[i] = current->meaning[j];
					left_node->synonym[i] = current->synonym[j];
					left_node->antonym[i] = current->antonym[j];
				}
				for(i=left_node->size+1, j=0 ; i<current->size+1 ; j++)
				{
					left_node->child[i] = current->child[j];
					current->child[j] = NULL;
				}
				left_node->size += current->size+1;
				current->size = 0;
				remove_from_top_root(top_root->word[left], top_root, current);
			}
			else if(right <= top_root->size)
			{
				node *right_node = top_root->child[right];
				current->word[current->size] = top_root->word[right-1];
				current->meaning[current->size] = top_root->meaning[right-1];
				current->synonym[current->size] = top_root->synonym[right-1];
				current->antonym[current->size] = top_root->antonym[right-1];
				for(i=current->size+1, j=0 ; j<right_node->size ; j++)
				{
					current->word[i] = right_node->word[j];
					current->meaning[i] = right_node->meaning[j];
					current->synonym[i] = right_node->synonym[j];
					current->antonym[i] = right_node->antonym[j];
				}
				for(i=current->size+1, j=0 ; j<right_node->size+1 ; j++)
				{
					current->child[i] = right_node->child[j];
					right_node->child[j] = NULL;
				}
				current->size += right_node->size+1;
				right_node->size = 0;
				remove_from_top_root(top_root->word[right-1], top_root, right_node);
			}
		}
		public :
		void search(string w)
		{
			node *current ;
			current = root;
			while(current->leaf != 1)
			{
				for(int i=0 ; i<current->size ; i++)
				{
					if(w < current->word[i])
					{
						current = current->child[i];
						break;
					}
					if(i == current->size-1)
					{
						current = current->child[i+1];
						break;
					}
				}
			}
			for(int i=0 ; i<current->size ; i++)
			{
				if(current->word[i] == w)
				{
					cout<<"\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t  Word  : "<<current->word[i]<<"\n\t\t\t\t\t\t\tMeaning : "<<current->meaning[i]<<"\n\t\t\t\t\t\t\tSynonym : "<<current->synonym[i]<<"\n\t\t\t\t\t\t\tAntonym : "<<current->antonym[i];
					return ;
				}
			}
			cout<<"\n\n\n\n\n\n\t\t\t\t\t\t\tSorry, word not found...!";
		}
		void display(node *current)
		{
			int i;
			if(current != NULL)
			{
				cout<<" | ";
				for(i=0 ; i<current->size ; i++)
				{
					cout<<current->word[i]<<" | ";
				}
				cout<<"\n\n";
				if(current->leaf != 1)
				{
					for(i=0 ; i<current->size+1 ; i++)
					{
						display(current->child[i]);
					}
				}
			}
		}
};
int main()
{
	BPTREE obj;
	obj.reading_from_file();
    string word, meaning, synonym, antonym;
    int choice;
    system("color 3F");
    cout<<"\t\t\t\t\t*********************************Dictionary*********************************\n";
    for(int i=0 ; ; i++)
    {
    	cout<<"\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tPress 1 to Add a Word.";
		cout<<"\n\t\t\t\t\t\t\tPress 2 to Search a Word.";
		cout<<"\n\t\t\t\t\t\t\tPress 3 to Delete a Word.";
		cout<<"\n\t\t\t\t\t\t\tPress 4 to See Whole Dictionary.";
		cout<<"\n\t\t\t\t\t\t\tPress any key other than above to Exit.";
		choice = getch();
		system("cls");
		switch(choice)
		{
			case '1' :
				cout<<"\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tEnter Word : ";
                cin>>word;
            	for(int i=0 ; i<word.length() ; i++)
            		if(word[i]>='A'&&word[i]<='Z')
            		    word[i]+=32;
				cout<<"\n\t\t\t\t\t\t\tEnter Meaning : ";
                cin>>meaning;
				cout<<"\n\t\t\t\t\t\t\tEnter Synonym : ";
                cin>>synonym;
				cout<<"\n\t\t\t\t\t\t\tEnter Antonym : ";
                cin>>antonym;
                obj.insert(word, meaning, synonym, antonym);
                break;
            case '2' :
            	cout<<"\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tEnter a Word to Search : ";
            	cin>>word;
            	for(int i=0 ; i<word.length() ; i++)
            		if(word[i]>='A'&&word[i]<='Z')
            		    word[i]+=32;
            	obj.search(word);
                break;
            case '3' :
            	cout<<"\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tEnter a Word to Delete : ";
            	cin>>word;
            	for(int i=0 ; i<word.length() ; i++)
            		if(word[i]>='A'&&word[i]<='Z')
            		    word[i]+=32;
            	obj.remove(word);
                break;
            case '4' :
            	obj.display(obj.root);
                break;
            default :
            	exit(0);
		}
	}
	return 0;
}
