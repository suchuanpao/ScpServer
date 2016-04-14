/*************************************************************************
 *
 *此代码使用的是linux kernel的链表写法
 *
 ************************************************************************/


#ifndef NULL
#define NULL ((void*) 0)
#endif

#define container_of(ptr,type,member) ({\
    const typeof( ( (type *)0 )->member ) *__mptr = (ptr); \
    (type*)((char*)__mptr - offsetof(type,member));})



typedef struct list_head{
    struct list_head * prev;
    struct list_head * next;
}ListHead;



#define LIST_HEAD_INIT(name) {&(name),&(name)}
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head * list)
{
    list->next = list;
    list->prev = list;
}

static inline void __list_add(struct list_head * new, struct list_head * prev, struct list_head * next)
{
    new->prev = prev;
    prev->next = new;
    new->next = next;
    next->prev = new;
}

static inline void list_add(struct list_head * new, struct list_head * head)
{
    __list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head * new, struct list_head * head)
{
    __list_add(new,head->prev,head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
    prev->next = next;;
    next->prev = prev;
}

static inline void list_del(struct list_head * entry)
{
    __list_del(entry->prev,entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

static inline void list_del_init(struct list_head * entry)
{
    __list_del(entry->prev,entry->next);
    INIT_LIST_HEAD(entry);
}

static inline void list_replace(struct list_head * new, struct list_head * old)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

static inline void list_replace_init(struct list_head * new, struct list_head * old)
{
    list_replace(new,old);
    INIT_LIST_HEAD(old);
}

static inline void list_move(struct list_head * list, struct list_head * head)
{
    __list_del(list->prev,list->next);
    list_add(list,head);
}

static inline void list_move_tail(struct list_head * list, struct list_head * head)
{
    __list_del(list->prev,list->next);
    list_add_tail(list,head);
}

static inline int list_is_last(struct list_head * list, struct list_head * head)
{
    return list->next == head;
}

static inline int list_empty(struct list_head * head)
{
    return head->next == head;
}


static inline int list_empty_careful(struct list_head * head)
{
    struct list_head * next = head->next;
    return (next == head) && (next == head->prev);
}

static inline int list_is_singular(struct list_head * head)
{
    return !list_empty(head) && (head->next == head->prev);
}

static inline void __list_cut_position(struct list_head * list, \
                                      struct list_head * head, \
                                      struct list_head * entry)
{
    struct list_head * temp = entry->next;
    list->next = head->next;
    list->next->prev = list;
    list->prev = entry;
    entry->next = list;
    head->next = temp;
    temp->prev = head;
}

//将head链表中的head头节点到entry节点之间的链表用list链接起来
static inline void list_cut_position(struct list_head * list, \
                                    struct list_head * head, \
                                    struct list_head * entry)
{
    if(list_empty(head)){
        return ;
    }
    if(list_is_singular(head) && \
        (head->next != entry) && \
       (head != entry)){
        return ;
    }
    if(entry == head){
        INIT_LIST_HEAD(list);
    }
    else{
        __list_cut_position(list,head,entry);
    }
}

static inline void __list_splice(struct list_head * list, \
                                struct list_head * prev, \
                                struct list_head * next)
{
    struct list_head * first = list->next;
    struct list_head * last = list->prev;
    
    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = first;
}

static inline void list_splice(struct list_head * list, \
                              struct list_head * head)
{
    if(!list_empty(list)){
        __list_splice(list,head,head->next);
    }
}

static inline void list_splice_tail(struct list_head * list, \
                                   struct list_head * head)
{
    if(!list_empty(list)){
        __list_splice(list,head->prev,head);
    }
}

static inline void list_splice_init(struct list_head * list, \
                                   struct list_head * head)
{
    if(!list_empty(list)){
        __list_splice(list,head,head->next);
    }
    INIT_LIST_HEAD(list);
}

static inline void list_splice_tail_init(struct list_head * list, \
                                        struct list_head * head)
{
    if(!list_empty(list)){
        __list_splice(list,head->prev,head);
    }
    INIT_LIST_HEAD(list);
}



#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)



#define list_first_entry(ptr, type, member) \
        list_entry((ptr)->next, type, member)

#define list_first_entry_or_null(ptr, type, member) \
        (!list_empty(ptr)?list_first_entry(ptr, ptr, member):NULL)

#define list_next_entry(pos, member) \
        list_entry((pos)->member.next, typeof(*(pos)), member)

#define list_prev_entry(pos, member) \
        list_entry((pos)->member.prev, typeof(*(pos)), member)

#define list_for_each(pos, head) \
        for(pos = (head)->next; pos != (head); pos = (pos)->next)

//遍历链表
#define __list_for_each(pos ,head) \
        for(pos = (head)->next; pos != head; pos = (pos)->next)

//逆序遍历链表
#define list_for_each_prev(pos, head) \
        for(pos = (head)->prev; pos != (head); pos = (pos)->prev)

//正序安全遍历链表
#define list_for_each_safe(pos, n, head) \
        for(pos = (head)->next, n = pos->next; pos != (head); pos = n; n = pos->next)

//向前遍历链表
#define list_for_each_prev_safe(pos, n, head) \
        for(pos = (head)->prev, n = pos->next; pos != (head); pos = n, n = pos->prev)

//正序遍历head开头的链表所在的节点
#define list_for_each_entry(pos, head, member) \
        for(pos = list_entry((head)->next, typeof(*pos), member); \
            &pos->member != (head); \
            pos = list_entry(pos->member.next, typeof(*pos), member))

//逆序遍历head开头的链表所在的节点
#define list_for_each_entry_reverse(pos, head, member) \
        for(pos = list_entry(head->prev, typeof(*pos), member); \
            &pos->member != (head); \
            pos = list_entry(pos->member.prev, typeof(*pos) ,member))

//不知道这个能用在哪里
#define list_prepare_entry(pos, head, member) \
        (pos) ? : list_entry(head, typeof(*pos), member)

//从链表中的pos所在节点的下一个节点开始遍历，到head结束
#define list_for_each_entry_continue(pos, head, member) \
        for(pos = list_entry(pos->member.next); \
            &pos->member != (head); \
           pos = list_entry(pos->member.next ,typeof(*pos), member))

//安全遍历链表所在的节点入口,我自己的版本
//#define list_for_each_entry_safe(pos, n, head, member) \
//        for(pos = list_entry(pos->member.next, typeof(*pos) ,member), n = pos->member.next; \
//           &pos->member.next != head; \
//           pos = list_entry(n, typeof(*pos), member), n = pos->member.next)
#define list_for_each_entry_safe(pos, n, head, member) \
        for(pos = list_entry(head->next, typeof(*pos), member), \
            n = list_entry(pos->member.next; typeof(*pos), member) \
            &pos->member != (head); \
           pos = n; n = list_entry(n->member.next, typeof(*n), member))
//安全遍历当前节pos->member点开始到head节点结束的链表内容.由pos获得每个节点值
#define list_for_each_entry_safe_continue(pos, n, head, member) \
        for(pos = list_entry(pos->member.next, typeof(*pos), member), \
           n = list_entry(pos->member.next, typeof(*pos), member); \
            &pos->member != (head); \
           pos = n, n = list_entry(n->member.next, typeof(*n), member))
//安全遍历当前节点pos->member开始到head节点结束的链表内容，from后缀说明pos是有值的
#define list_for_each_entry_safe_from(pos, n, head, member) \
        for(n = list_entry(pos->member.next, typeof(*pos), member), \
           n = list_entry(pos->member.next, typeof(*pos), member); \
            &pos->member != (head); \
           pos = n, n = list_entry(n->member.next, typeof(*n), member))
//安全倒序遍历每一个节点
#define list_for_each_entry_safe_reverse(pos, n, head, member) \
        for(pos = list_entry(head->next, typeof(*pos), member), \
            n = list_entry(pos->member.next, typeof(*pos), member); \
            &pos->member != (head); \
            pos = n, n = list_entry(n->member.next, typeof(*pos), member)
//通过n获取下一节点type结构的首地址
#define list_safe_reset_next(pos,n,member) \
            n = list_entry(pos->member.next, typeof(*pos), member);
            
