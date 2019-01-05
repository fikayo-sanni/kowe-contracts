#include "cxtapp.hpp"
#include <time.h>
#include <ctime>
/**
* This file describes a login and signup function for a semi decentralized design schema for EOS dev
*/


void cxtapp::login(
  name username,
  asset init
){

  require_auth(username);

  profile_table profiles(_self,_self.value);//init profile table

  auto iter = profiles.find(username.value);

  accounts to_acnts( _self, username.value);
  auto to = to_acnts.find(init.symbol.raw());

  if(to==to_acnts.end()){
    //issue(username, asset(10, init.symbol), "account setup");
    //issuevest(username, asset(10, init.symbol), "account setup");
  }

  if(iter== profiles.end()){
    profiles.emplace(_self,[&](auto & row){//create user
      row.username = username;
      row.created = now();
      row.last_login = now();
    });
  }else{
    profiles.modify(iter,_self,[&](auto & row){
      row.last_login = now();
    });
  }

  print("Logged in");

}

void cxtapp::updateuser(
  name username,
  string fullname,
  string bio,
  string location
){
  require_auth(username);

  profile_table profiles(_self,_self.value);//init profile table

  auto itr = profiles.find(username.value);

  eosio_assert(itr!=profiles.end(),"user not found!");

  profiles.modify(itr, _self,[&](auto & row){//update profile
    row.fullname = fullname;
    row.bio = bio;
    row.location = location;
    row.updated = now();
  });

  print("profile updated");

}


void cxtapp::updatepix(
    name username,
    string bucket,
    string object,
    string type
){
  require_auth(username);

  profile_table profiles(_self,_self.value);//init profile table

  auto itr = profiles.find(username.value);

  eosio_assert(itr!=profiles.end(),"user not found!");

  if(type=="wallpaper"){
    profiles.modify(itr, _self,[&](auto & row){//update profile
      row.wallpaper_bucket = bucket;
      row.wallpaper_object = object;
      row.updated = now();
    });
  }else{
    profiles.modify(itr, _self,[&](auto & row){//update profile
      row.dp_bucket = bucket;
      row.dp_object = object;
      row.updated = now();
    });
  }

  print("profile updated");

}


void cxtapp:: createdraft(
  uint64_t draft_id,
  name username,
  string title,
  string permlink,
  string bucket,
  string object,
  string tags,
  string genre,
  string synopsis,
  string language,
  string copyright,
  bool adult
){

  require_auth(username);

  drafts_table drafts(_self,_self.value);//init drafts table

  drafts.emplace(_self,[&](auto & row){//insert draft
    row.draft_id = draft_id;
    row.username = username;
    row.permlink = permlink;
    row.title = title;
    row.bucket = bucket;
    row.object = object;
    row.tags = tags;
    row.genre = genre;
    row.synopsis = synopsis;
    row.language = language;
    row.copyright = copyright;
    row.adult = adult;
    row.created = now();
  });

  print("book created");

}

void cxtapp::updatedraft(
  name username,
  uint64_t draft_id,
  string title,
  string bucket,
  string object,
  string tags,
  string genre,
  string synopsis,
  string language,
  string copyright,
  bool adult
){

  require_auth(username);

  drafts_table drafts(_self,_self.value);//init drafts table

  auto itr = drafts.find(draft_id);

  eosio_assert( itr!=drafts.end(),"draft not found");
  eosio_assert(itr->username == username, "user unauthorized");

  drafts.modify(itr, _self, [&](auto & row){//update draft
    row.title = title;
    row.bucket = bucket;
    row.object = object;
    row.tags = tags;
    row.genre = genre;
    row.synopsis = synopsis;
    row.language = language;
    row.copyright = copyright;
    row.adult = adult;
    row.updated = now();
  });

  print("draft updated");

}


void cxtapp::addchapter(
  name username,
  uint64_t draft_id,
  uint64_t chapter_id,
  string title
){
  require_auth(username);

  drafts_table drafts(_self,_self.value);//init drafts table

  auto itr = drafts.find(draft_id);

  eosio_assert( itr!=drafts.end(),"draft not found");
  eosio_assert(itr->username == username, "user unauthorized");

  chapters_table chapters(_self,_self.value);

  chapters.emplace(_self, [&](auto & row){//emplace content
    row.draft_id = draft_id;
    row.chapter_id = chapter_id;
    row.title = title;
    row.created = now();
  });

  print("success");
}


void cxtapp::addcontent(
  name username,
  uint64_t draft_id,
  uint64_t chapter_id,
  uint64_t word_count,
  string content
){
  require_auth(username);

  drafts_table drafts(_self,_self.value);//init drafts table

  auto itr = drafts.find(draft_id);

  eosio_assert( itr!=drafts.end(),"draft not found");
  eosio_assert(itr->username == username, "user unauthorized");

  chapters_table chapters(_self, _self.value);
  auto iter = chapters.find(chapter_id);//find chapter

  eosio_assert( iter!=chapters.end(), "chapter not found");
  eosio_assert(iter->draft_id==draft_id, "book and chapter unmatch");


  drafts.modify(itr, _self, [&](auto & row){//update draft
    row.updated = now();
  });

  chapters.modify(iter, _self, [&](auto & row){//update chapter
      row.word_count = word_count;
      row.content = content;
      row.updated = now();
  });

  print("draft updated");
}

void cxtapp::publish(
  name username,
  uint64_t draft_id
){
  require_auth(username);

  drafts_table drafts(_self,_self.value);//init drafts Table

  auto itr = drafts.find(draft_id);

  eosio_assert( itr!=drafts.end(),"draft not found");
  eosio_assert(itr->username == username, "user unauthorized");

  books_table books(_self,_self.value);

  books.emplace(_self,[&](auto & row){
    row.book_id = itr->draft_id;
    row.username = itr->username;
    row.permlink = itr->permlink;
    row.title = itr->title;
    row.bucket = itr->bucket;
    row.object = itr->object;
    row.tags = itr->tags;
    row.genre = itr->genre;
    row.synopsis = itr->synopsis;
    row.language = itr->language;
    row.copyright = itr->copyright;
    row.adult = itr->adult;
    row.updated = now();
    row.created = now();
  });

  drafts.erase(itr);

  print("book published");

}

void cxtapp::addreview(
  uint64_t review_id,
  uint64_t book_id,
  name username,
  uint32_t rating,
  string review
){
  require_auth(username);

  reviews_table reviews(_self, _self.value);//init reviews table

  reviews.emplace(_self, [&](auto & row){
    row.review_id = review_id;
    row.book_id = book_id;
    row.username = username;
    row.rating = rating;
    row.review = review;
  });

  print("review added");
}

void cxtapp::follow(
  uint64_t follow_id,
  name username,
  name followed
){

  require_auth(username);//auth application

  follow_table follows(_self,_self.value);//init follow table

  auto user_index = follows.get_index<name("byuser")>();//get index by users

  auto itr = user_index.find(username.value);

  for (; itr != user_index.end(); ++itr)
    eosio_assert(itr->username != followed,"priorly followed");


  follows.emplace(_self,[&](auto & row){
    row.follow_id = follow_id;
    row.username = username;
    row.followed = followed;
    row.created = now();
  });

  print("followed");

}


void cxtapp::unfollow(
  name username,
  uint64_t follow_id
){
  require_auth(username);//auth application

  follow_table follows(_self,_self.value);

  auto itr = follows.find(follow_id);

  eosio_assert(itr->username == username,"user unauthorized");

  follows.erase(itr);

  eosio_assert(itr!=follows.end(),"operation failed");

  print("unfollowed");

}


void cxtapp::createrl(
  uint64_t list_id,
  name username,
  string collection_name,
  string description
){
  require_auth(username);//auth application

  readlist_table readlists(_self,_self.value);

  readlists.emplace(_self,[&](auto & row){
    row.list_id = list_id;
    row.username = username;
    row.collection_name = collection_name;
    row.description = description;
  });

  print("collection created");
}


void cxtapp::updaterl(
  name username,
  uint64_t list_id,
  string collection_name
){

  require_auth(username);//auth user

  readlist_table readlists(_self,_self.value);

  auto itr = readlists.find(list_id);

  eosio_assert( itr != readlists.end(), "collection not found" );

  eosio_assert( itr->username==username, "user unauthorized" );

  readlists.modify(itr,_self,[&](auto & row){
    row.collection_name = collection_name;
  });

  print("collection modified");
}

void cxtapp::insertrl(
  name username,
  uint64_t list_id,
  uint64_t book_id,
  uint64_t item_id
){
  require_auth(username);
  readlist_table readlists(_self,_self.value);//init readlists table
  auto itr = readlists.find(list_id);

  eosio_assert( itr != readlists.end(), "collection not found");

  eosio_assert( itr->username == username, "user unauthorized");


  readlistitems_table items(_self,_self.value);

  auto list_index = items.get_index<name("bylist")>();

  auto itrr = list_index.find(list_id);

  for(; itrr != list_index.end(); ++itrr)//check for pre existing sessions on the same platform
      eosio_assert(itrr->book_id != book_id, "record exists");//return existing session ti the client

  items.emplace(_self,[&](auto & row){
    row.item_id = item_id;
    row.list_id = list_id;
    row.book_id = book_id;
  });

}

void cxtapp::create( name issuer,
                    asset        maximum_supply )
{
    require_auth( _self );

    auto sym = maximum_supply.symbol;
    eosio_assert( sym.is_valid(), "invalid symbol name" );
    eosio_assert( maximum_supply.is_valid(), "invalid supply");
    eosio_assert( maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable( _self, sym.raw() );
    auto existing = statstable.find( sym.raw() );
    eosio_assert( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( _self, [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = issuer;
    });
}


void cxtapp::issue( name to, asset quantity, string memo )
{
    auto sym = quantity.symbol;
    eosio_assert( sym.is_valid(), "invalid symbol name" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

    auto sym_name = sym.raw();
    stats statstable( _self, sym_name );
    auto existing = statstable.find( sym_name );
    eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;

    //require_auth( st.issuer );
    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must issue positive quantity" );

    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, _self, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );

    if( to != st.issuer ) {
       SEND_INLINE_ACTION( *this, transfer, {st.issuer,name("active")}, {st.issuer, to, quantity, memo} );
    }
}

void cxtapp::issuevest( name to, asset quantity, string memo )
{
    auto sym = quantity.symbol;
    eosio_assert( sym.is_valid(), "invalid symbol name" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

    auto sym_name = sym.raw();
    stats statstable( _self, sym_name );
    auto existing = statstable.find( sym_name );
    eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;

    //require_auth( st.issuer );
    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must issue positive quantity" );

    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, _self, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );

    if( to != st.issuer ) {
       sub_balance(st.issuer, quantity);
       strict_vest(to,quantity);
    }
}

void cxtapp::transfer( name from,
                      name to,
                      asset        quantity,
                      string       memo )
{
    eosio_assert( from != to, "cannot transfer to self" );
    require_auth( from );
    eosio_assert( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.raw();
    stats statstable( _self, sym );
    const auto& st = statstable.get( sym );

    require_recipient( from );
    require_recipient( to );

    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );


    sub_balance( from, quantity );
    add_balance( to, quantity, from );
}

void cxtapp::vest(name username,
                asset quantity)
{
  require_auth(username);
  auto sym = quantity.symbol.raw();

  stats statstable( _self, sym );
  const auto& st = statstable.get( sym );
  eosio_assert( quantity.is_valid(), "invalid quantity" );
  eosio_assert( quantity.amount > 0, "must vest positive quantity" );
  eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

  vest_balance(username, quantity);
}

void cxtapp::unvesting( name username, asset quantity){
  require_auth(username);
  auto sym = quantity.symbol.raw();

  stats statstable( _self, sym );
  const auto& st = statstable.get( sym );
  eosio_assert( quantity.is_valid(), "invalid quantity" );
  eosio_assert( quantity.amount > 0, "must unvest positive quantity" );
  eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

  accounts from_acnts( _self, username.value );

  const auto& from = from_acnts.get( quantity.symbol.raw(), "no balance object found" );
  eosio_assert( from.vested.amount >= quantity.amount, "overdrawn balance" );

  unvests unvest_table(_self, _self.value);

  auto iter = unvest_table.find(username.value);

  if(iter== unvest_table.end()){
    unvest_table.emplace(_self,[&](auto & row){
      row.username = username;
      row.quantity = quantity;
      row.outstanding = 12;
      row.start_date = now();
    });
  }else{
    unvest_table.modify(iter,_self, [&](auto & row){
      row.quantity = quantity;
      row.outstanding = 12;
      row.start_date = now();
    });
  }
}

//BUGGY AF
void cxtapp::liquify( name username, symbol_code sym){
  require_auth(username);

  //if(sym == name("CXT")){
    unvests unvest_table(_self, _self.value);

    auto iter = unvest_table.find(username.value);
    eosio_assert(iter!=unvest_table.end(),"record not found");

    uint32_t claimed = 12 - iter->outstanding;

    uint32_t passed = 1; //difftime(now(),(iter->start_date)) / 2592000; //fetch number of months passed

   auto claim = (iter->quantity.amount) / 12;

   auto total_claim = claim * (passed-claimed);
    if(iter->outstanding!=1){
      unvest_table.modify(iter,_self, [&](auto & row){
        row.outstanding = 12-passed;
      });
    }else{
      unvest_table.erase(iter);
    }

   accounts from_acnts( _self,username.value );

   const auto& from = from_acnts.get( sym.raw(), "no balance object found" );
   eosio_assert( from.balance.amount > total_claim, "overdrawn balance" );

   from_acnts.modify(from, _self, [&]( auto& a ) {
      a.balance += asset(total_claim, iter->quantity.symbol);
      a.vested -= asset(total_claim, iter->quantity.symbol);
    });
  /*}else{
    eosio_assert(1==2,"invalid token");//throw exception
  }*/
}

void cxtapp::vote(name voter, uint64_t book_id, symbol sym, uint64_t vote_id){
  require_auth(voter);

  //check if book exists and fetch the book's writer USERNAME
   books_table books(_self, _self.value);

  auto itr = books.find(book_id);
  eosio_assert(itr != books.end(), "cannot find book_id");

  //check last vote
  votes vote_table(_self, _self.value);

  auto user_index = vote_table.get_index<name("byuser")>();

  auto iter = user_index.find(voter.value);

  for(; iter!=user_index.end(); iter++){
    double diff_time = 1;//difftime(now(), iter->timestamp);
    eosio_assert(diff_time > 5184000, "You can only vote a book once in two months");
  }
  //fetch current voting power status
  voting_power vp(_self, _self.value);

  auto iterr = vp.find(voter.value);

  uint32_t left;
  if(iterr==vp.end()){
    vp.emplace(_self,[&](auto & row){
      row.username = voter;
      row.start_date = now();
      row.left = 59;
    });

    left = 60;
  }else if (/**difftime(now(), iterr->start_date) **/left> 2592000){
    vp.modify(iterr,_self,[&](auto & row){
      row.username = voter;
      row.start_date = now();
      row.left = 59;
    });

    left = 60;
  }else{
    vp.modify(iterr,_self,[&](auto & row){
      row.left -= 1;
    });
    left = iterr->left-1;
  }

  //check current vested balance
  accounts from_acnts( _self, voter.value );

  const auto& user = from_acnts.get(sym.code().raw(), "no initial balance found");

  asset supply = get_supply(sym.code());

  int64_t vote_value = (left/1830) * (user.vested.amount/12 ) * 0.2 * 0.9;

  asset vote_asset = asset(vote_value, sym);
  asset owner_asset = asset(0.3*vote_value, sym);
  asset reader_asset = asset(0.2*vote_value, sym);

  //create vote
  vote_table.emplace(_self,[&](auto & row){
    row.vote_id = vote_id;
    row.book_id = book_id;
    row.username = voter;
    row.value = vote_asset;
    row.timestamp = now();
  });

  claims claims_table(_self,_self.value);

  claims_table.emplace(_self, [&](auto & row){
    row.claim_id = claims_table.available_primary_key();
    row.vote_id = vote_id;
    row.liquid = owner_asset;
    row.vested = owner_asset;
    row.username = itr->username;
    row.owner = true;
    row.timestamp = now();
  });

  claims_table.emplace(_self, [&](auto & row){
    row.claim_id = claims_table.available_primary_key();
    row.vote_id = vote_id;
    row.liquid = reader_asset;
    row.vested = reader_asset;
    row.username = voter;
    row.owner = false;
    row.timestamp = now();
  });
   //create claims for voter and writer
}

void cxtapp::claimreward(name username, uint64_t vote_id){
  require_auth(username);

  claims claims_table(_self,_self.value);

  auto votes_index = claims_table.get_index<name("byvote")>();

  auto iter = votes_index.find(vote_id);

  eosio_assert(iter==votes_index.end(),"claim doesn't match a valid vote");

  for(; iter!=votes_index.end(); iter++){
    if(iter->username == username){
      issue( username, iter->liquid, "reward" );
      issuevest( username, iter->vested, "vested reward" );
      //claims_table.erase(iter);
    }
  }

}


void cxtapp::strict_vest( name owner, asset value ) {
  accounts to_acnts( _self, owner.value );
  auto to = to_acnts.find( value.symbol.raw() );
  if( to == to_acnts.end() ) {
     to_acnts.emplace( _self, [&]( auto& a ){
       a.vested = value;
     });
  } else {
     to_acnts.modify( to, _self, [&]( auto& a ) {
       a.vested += value;
     });
  }
}

void cxtapp::vest_balance( name owner, asset value ) {
  require_auth(owner);

   accounts from_acnts( _self, owner.value );

   const auto& from = from_acnts.get( value.symbol.raw(), "no balance object found" );
   eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );

   from_acnts.modify( from, owner, [&]( auto& a ) {
      a.balance -= value;
      a.vested += value;
    });

}

void cxtapp::sub_balance( name owner, asset value ) {
   accounts from_acnts( _self, owner.value );

   const auto& from = from_acnts.get( value.symbol.raw(), "no balance object found" );
   eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );


   from_acnts.modify( from, owner, [&]( auto& a ) {
     a.balance -= value;
   });

}

void cxtapp::add_balance( name owner, asset value, name ram_payer )
{
   accounts to_acnts( _self, owner.value );
   auto to = to_acnts.find( value.symbol.raw() );
   if( to == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, _self, [&]( auto& a ) {
        a.balance += value;
      });
   }
}


void cxtapp::followtag(
  uint64_t follow_id,
  name username,
  string tag
){

}

void cxtapp::unfollowtag(
  uint64_t follow_id,
  name username
){

}


void cxtapp::newcommune(
  name username,
  string name,
  string description,
  string permlink
){



}

void cxtapp::preview(
  uint64_t preview_id,
  name username,
  uint64_t book_id
){

  require_auth(username);

  preview_table previews(_self, _self.value);

  auto user_index = previews.get_index<name("byuser")>();//get index by users

  auto itr = user_index.find(username.value);

  for (; itr != user_index.end(); ++itr)
    eosio_assert(itr->book_id != book_id,"priorly previewed");

  previews.emplace(_self,[&](auto & row){//create user
    row.preview_id = preview_id;
    row.username = username;
    row.book_id = book_id;
    row.created = now();
  });

}

void cxtapp::bookmark(
  uint64_t mark_id,
  name username,
  uint64_t book_id,
  uint32_t page
){

  require_auth(username);

  bookmark_table bookmarks(_self, _self.value);

  bookmarks.emplace(_self,[&](auto & row){//create user
    row.mark_id = mark_id;
    row.username = username;
    row.book_id = book_id;
    row.page = page;
    row.updated = now();
    row.created = now();
  });

}
