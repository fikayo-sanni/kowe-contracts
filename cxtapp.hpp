#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>
#include <string>
#include <stdio.h>
#include <time.h>
#include <ctime>

using namespace eosio;
using std::string;
using std::time_t;

namespace eosiosystem {
   class system_contract;
}

namespace eosio {
  CONTRACT cxtapp : public contract {

      using contract::contract;

      public:
          // @abi action
          void login(
            const name username,
            const asset init
          );

          // @abi action
          void updateuser(
            const name username,
            const string fullname,
            const string bio,
            const string location
          );

          // @abi action
          void updatepix(
            const name username,
            const string bucket,
            const string object,
            const string type
          );

          // @abi action
          void createdraft(
            const uint64_t draft_id,
            const name username,
            const string title,
            const string permlink,
            const string bucket,
            const string object,
            const string tags,
            const string genre,
            const string synopsis,
            const string language,
            const string copyright,
            const bool adult
          );

          // @abi action
          void updatedraft(
            const name username,
            const uint64_t draft_id,
            const string title,
            const string bucket,
            const string object,
            const string tags,
            const string genre,
            const string synopsis,
            const string language,
            const string copyright,
            const bool adult
          );

          // @abi action
          void savebook(
            const name username,
            const uint64_t draft_id,
            const string title,
            const string cover,
            const string genre,
            const string synopsis,
            const string content,
            const string language
          );

          // @abi action
          void addchapter(
            const name username,
            const uint64_t draft_id,
            const uint64_t chapter_id,
            const string title
          );


          // @abi action
          void addcontent(
            const name username,
            const uint64_t draft_id,
            const uint64_t chapter_id,
            const uint64_t word_count,
            const string content
          );

          // @abi action
          void publish(
            const name username,
            const uint64_t draft_id
          );

          // @abi action
          void follow(
            const uint64_t follow_id,
            const name username,
            const name followed
          );

          // @abi action
          void unfollow(
            const name username,
            const uint64_t follow_id
          );

          // @abi action
          void addreview(
            const uint64_t review_id,
            const uint64_t book_id,
            const name username,
            const uint32_t rating,
            const string review
          );

          // @abi action
          void createrl(
            uint64_t list_id,
            const name username,
            const string collection_name,
            const string description
          );

          // @abi action
          void updaterl(
            const name username,
            const uint64_t list_id,
            const string collection_name
          );

          // @abi action
          void insertrl(
            const name username,
            const uint64_t list_id,
            const uint64_t book_id,
            const uint64_t item_id
          );

          // @abi action
          void followtag(
            const uint64_t follow_id,
            const name username,
            const string tag
          );

          // @abi action
          void unfollowtag(
            const uint64_t follow_id,
            const name username
          );

          /**
          * Token functions
          */

          // @abi action
          void create(
            const name issuer,
            const asset        maximum_supply
          );

         // @abi action
          void issue(
            const name to,
            const asset quantity,
            const string memo
          );

          // @abi action
          void issuevest(
            const name to,
            const asset quantity,
            const string memo
          );

          // @abi action
          void transfer(
            const name from,
            const name to,
            const asset  quantity,
            const string  memo
          );

         void strict_vest(
           const name username,
           const asset quantity
         );

         // @abi action
          void vest(
            const name username,
            const asset quantity
          );

         // @abi action
          void unvesting(
            const name username,
            const asset quantity
          );

          // @abi action
          void liquify(
            const name username,
            const symbol_code sym
          );

          // @abi action
          void vote(
            const name voter,
            const uint64_t book_id,
            const symbol sym,
            const uint64_t vote_id
          );

          // @abi action
          void claimreward(
            const name username,
            const uint64_t vote_id
          );

          // @abi action
          void preview(
            const uint64_t preview_id,
            const name username,
            const uint64_t book_id
          );

          // @abi action
          void bookmark(
            const uint64_t mark_id,
            const name username,
            const uint64_t book_id,
            const uint32_t page
          );

          inline asset get_supply( symbol_code sym )const;

          inline asset get_balance( name owner, symbol_code sym )const;


          /**
          * UnImplemented
          */

          void newcommune(
            const name username,
            const string name,
            const string description,
            const string permlink
          );

          void followcomm(//follow community

          );

          void newcontest(
            const name username,
            const string name,
            const string description,
            const string permlink
          );

          void newevent(
            const name username,
            const string banner,
            const string community_name,
            const string description,
            const string permlink
          );

          void newrsvp(
            const name username,
            const uint64_t event_id
          );

          void newpost(
            const name username,
            const uint64_t community_permlink,
            const string content
          );

          void newread(
            const name username,
            const uint64_t book_id
          );

          void updatecontest(
            const name username,
            const uint64_t contest_id,
            const string name,
            const string description
          );

          void deletepost(
            const name username,
            const uint64_t post_id
          );


      private:

        // @abi table profiles i64
        struct profile{
          name username;
          string fullname;
          string bio;
          string location;
          string dp_bucket;
          string dp_object;
          string wallpaper_bucket;
          string wallpaper_object;
          time_t created;
          time_t updated;
          time_t last_login;

          auto primary_key() const {return username.value;}
          EOSLIB_SERIALIZE(profile,(username)(fullname)(bio)(location)(dp_bucket)(dp_object)(wallpaper_bucket)(wallpaper_object)(created)(updated)(last_login));
        };

        typedef multi_index<name("profiles"), profile> profile_table;


        // @abi table drafts i64
        struct draft{
          uint64_t draft_id;
          name username;
          string permlink;
          string title;
          string bucket;
          string object;
          string tags;
          string genre;
          string synopsis;
          string language;
          string copyright;
          bool adult;
          time_t updated;
          time_t created;

          auto primary_key() const {return draft_id;}

          EOSLIB_SERIALIZE(draft, (draft_id)(username)(permlink)(title)(bucket)(object)(tags)(genre)(synopsis)(language)(copyright)(adult)(updated)(created))
        };

        typedef multi_index<name("drafts"), draft> drafts_table;

        // @abi table chapters i64
        struct chapter{
          uint64_t chapter_id;
          uint64_t draft_id;
          string title;
          string content;
          uint64_t word_count;
          time_t updated;
          time_t created;

          auto primary_key() const {return chapter_id;}

          EOSLIB_SERIALIZE(chapter, (chapter_id)(draft_id)(title)(content)(word_count)(updated)(created))
        };

        typedef multi_index<name("chapters"), chapter> chapters_table;

        // @abi table reviews i64
        struct reviews{
          uint64_t review_id;
          uint64_t book_id;
          name username;
          uint32_t rating;
          string review;

          auto primary_key() const {return review_id;}

          EOSLIB_SERIALIZE(reviews, (review_id)(book_id)(username)(rating)(review))
        };

        typedef multi_index<name("reviews"), reviews> reviews_table;

        // @abi table books i64
        struct book{
          uint64_t book_id;
          string bucket;
          name username;
          string permlink;
          string title;
          string object;
          string tags;
          string genre;
          string synopsis;
          string language;
          string copyright;
          bool adult;
          time_t updated;
          time_t created;

          auto primary_key() const {return book_id;}

          EOSLIB_SERIALIZE(book, (book_id)(username)(permlink)(title)(bucket)(object)(tags)(genre)(synopsis)(language)(copyright)(adult)(updated)(created))
        };

        typedef multi_index<name("books"), book> books_table;

        // @abi table follows i64
        struct following{
          uint64_t follow_id;
          name username;
          name followed;
          time_t created;

          auto primary_key() const {return follow_id;}
          uint64_t by_user() const {return username.value;}

          EOSLIB_SERIALIZE(following, (follow_id)(username)(followed)(created))
        };

        typedef multi_index<name("follows"),following,
        indexed_by<name("byuser"), const_mem_fun< following, uint64_t, &following::by_user >>> follow_table;

        // @abi table followtags i64
        struct tagfollow{
          uint64_t follow_id;
          name username;
          string tag;
          time_t created;

          auto primary_key() const {return follow_id;}
          uint64_t by_user() const {return username.value;}

          EOSLIB_SERIALIZE(tagfollow, (follow_id)(username)(tag)(created))
        };

        typedef multi_index<name("followtags"),tagfollow,
        indexed_by<name("byuser"), const_mem_fun< tagfollow, uint64_t, &tagfollow::by_user >>> followtags_table;

        // @abi table readlists i64
        struct readlist{
          uint64_t list_id;
          name username;
          string collection_name;
          string description;

          auto primary_key() const {return list_id;}
          uint64_t by_user() const {return username.value;}

          EOSLIB_SERIALIZE(readlist, (list_id)(username)(collection_name))
        };

        typedef multi_index<name("readlists"),readlist,
        indexed_by<name("byuser"), const_mem_fun< readlist, uint64_t, &readlist::by_user >>> readlist_table;

      // @abi table rlitems i64
      struct rlitem{
        uint64_t item_id;
        uint64_t list_id;
        uint64_t book_id;

        auto primary_key() const {return item_id;}
        uint64_t by_list() const {return list_id;}

        EOSLIB_SERIALIZE(rlitem, (item_id)(list_id)(book_id))
      };

      typedef multi_index<name("rlitems"),rlitem,
      indexed_by<name("bylist"), const_mem_fun< rlitem, uint64_t, &rlitem::by_list >>> readlistitems_table;

      // @abi table  bookmarks i64
      struct bmark{
        uint64_t mark_id;
        name username;
        uint64_t book_id;
        uint32_t page;
        time_t created;
        time_t updated;

        auto primary_key() const {return mark_id;}
        uint64_t by_user() const {return username.value;}

        EOSLIB_SERIALIZE(bmark, (mark_id)(username)(book_id)(page)(created)(updated))
      };

      typedef multi_index<name("bookmarks"),bmark,
      indexed_by<name("byuser"), const_mem_fun< bmark, uint64_t, &bmark::by_user >>> bookmark_table;

      // @abi table  previews i64
      struct prevtable{
        uint64_t preview_id;
        name username;
        uint64_t book_id;
        time_t created;

        auto primary_key() const {return preview_id;}
        uint64_t by_user() const {return username.value;}

        EOSLIB_SERIALIZE(prevtable, (preview_id)(username)(book_id)(created))
      };

      typedef multi_index<name("previews"),prevtable,
      indexed_by<name("byuser"), const_mem_fun< prevtable, uint64_t, &prevtable::by_user >>> preview_table;


      struct community{
        uint64_t community_id;
        name founder;
        string name;
        string description;
        string permlink;

        auto primary_key() const {return community_id;}

        EOSLIB_SERIALIZE(community, (community_id)(founder)(name)(description)(permlink))
      };

      typedef multi_index<name("communities"), community> communities;

      /**
      *Token Private Structs
      */
      // @abi table vp i64
      struct vp {
        name username;
        time_t start_date;
        uint32_t left;

        auto primary_key()const { return username.value; }
        EOSLIB_SERIALIZE(vp, (username)(start_date)(left));
      };

      // @abi table votes i64
      struct votes_table{
        uint64_t vote_id;
        uint64_t book_id;
        name username;
        asset value;
        time_t timestamp;

        auto primary_key()const {return vote_id; }
        uint64_t by_user() const {return username.value;}

        EOSLIB_SERIALIZE(votes_table, (vote_id)(book_id)(username)(value)(timestamp));
      };

      // @abi table claims i64
      struct claim{
        uint64_t claim_id;
        uint64_t vote_id;
        asset liquid;
        asset vested;
        name username;
        bool owner;
        time_t timestamp;

        auto primary_key()const {return claim_id;}
        uint64_t by_vote() const {return vote_id;}
      };

     // @abi table
     struct account {
        asset    balance;
        asset    vested;
        uint64_t primary_key()const { return balance.symbol.raw(); }
     };

     // @abi table
     struct currency_stats {
        asset          supply;
        asset          max_supply;
        name   issuer;

        uint64_t primary_key()const { return supply.symbol.raw(); }
     };

     // @abi table
     struct unvest {
       name username;
       asset        quantity;
       uint32_t     outstanding;
       time_t       start_date;

       auto primary_key() const {return username.value;}

       EOSLIB_SERIALIZE(unvest, (username)(quantity)(outstanding)(start_date));
     };

     typedef eosio::multi_index<name("votes"), votes_table,
     indexed_by<name("byuser"), const_mem_fun< votes_table, uint64_t, &votes_table::by_user >>> votes;
     typedef multi_index<name("claims"),claim,
     indexed_by<name("byvote"), const_mem_fun< claim, uint64_t, &claim::by_vote >>> claims;
     typedef eosio::multi_index<name("vp"), vp> voting_power;
     typedef eosio::multi_index<name("unvests"), unvest> unvests;
     typedef eosio::multi_index<name("accounts"), account> accounts;
     typedef eosio::multi_index<name("stat"), currency_stats> stats;

     void sub_balance( name owner, asset value );
     void add_balance( name owner, asset value, name ram_payer );
     void vest_balance( name owner, asset value );
  };
  asset cxtapp::get_supply( symbol_code sym )const
  {
     stats statstable( _self, sym.raw() );
     const auto& st = statstable.get( sym.raw() );
     return st.supply;
  }

  asset cxtapp::get_balance( name owner, symbol_code sym )const
  {
     accounts accountstable( _self, owner.value );
     const auto& ac = accountstable.get( sym.raw() );
     return ac.balance;
  }
}
EOSIO_DISPATCH(cxtapp, (login)(updateuser)(updatepix)(createdraft)(updatedraft)(addchapter)(addcontent)(publish)(follow)(unfollow)(addreview)(createrl)(insertrl)(followtag)(unfollowtag)(preview)(bookmark)(create)(issue)(issuevest)(transfer)(vest)(unvesting)(liquify)(vote)(claimreward))
